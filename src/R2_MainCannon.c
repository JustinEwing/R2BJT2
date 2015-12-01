
/*
 * File: FindOpponentHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "R2_BJT2_HSM.h"
#include "R2_MainCannon.h"
#include "LauncherMotor.h"
#include "LauncherServo.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_R2MainCannon_STATES(STATE) \
        STATE(InitR2MainCannonState)    \
        STATE(InitCannon) \
        STATE(Spooling) \
        STATE(Load) \
        STATE(FIRE) \
        STATE(WiggleLeft) \
        STATE(WiggleRight) \


#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_R2MainCannon_STATES(ENUM_FORM)
} R2MainCannonState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_R2MainCannon_STATES(STRING_FORM)
};

//#define MAINCANNON_DEBUG_VERBOSE
#ifdef MAINCANNON_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static R2MainCannonState_t CurrentState = InitR2MainCannonState;
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitR2MainCannon(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitR2MainCannon(void) {
    ES_Event returnEvent;

    dbprintf("Init Cannon FSM\n");
    R2CloseBarrel();
    R2LauncherMotorSpeed(0);

    CurrentState = InitR2MainCannonState;
    returnEvent = RunR2MainCannon(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunR2MainCannon(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */


// TODO: Need to integrate beacon events -> beacon detected & beacon lost

ES_Event RunR2MainCannon(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    R2MainCannonState_t nextState;
    static uint8_t firecount = 0;
    static uint8_t wiggleCount = 0;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitR2MainCannonState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT) {

                //Set up the cannon
                dbprintf("Initing Cannon\n");

                nextState = InitCannon;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case InitCannon:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2CloseBarrel();
                        R2LauncherMotorSpeed(0);
                        ES_Timer_InitTimer(GUN_TIMER, 20);
                        ThisEvent.EventType = ES_NO_EVENT;
                        dbprintf("psudo cannon init\n");
                        break;

                        //                    case BEACON_FOUND:
                        //                        nextState = Spooling;
                        //                        makeTransition = TRUE;
                        //                        ThisEvent.EventType = ES_NO_EVENT;
                        //                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case GUN_TIMER:
                                nextState = Spooling;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(GUN_TIMER);
                        break;

                    default: break;
                }
            }
            break;

        case Spooling:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("Spooling motors\n");
                        R2LauncherMotorSpeed(30);
                        ES_Timer_InitTimer(GUN_TIMER, 3500);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case GUN_TIMER:
                                nextState = Load;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            default:break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(GUN_TIMER);
                        break;


                    default: break;
                }
            }
            break;

        case WiggleRight:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(GUN_TIMER, 3500);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case GUN_TIMER:
                                nextState = Load;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            default:break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(GUN_TIMER);
                        break;


                    default: break;
                }
            }
            break;

        case WiggleLeft:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("Spooling motors\n");
                        R2LauncherMotorSpeed(30);
                        ES_Timer_InitTimer(GUN_TIMER, 3500);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case GUN_TIMER:
                                nextState = Load;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            default:break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(GUN_TIMER);
                        break;


                    default: break;
                }
            }
            break;

        case Load:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("Loading Cannon\n");
                        R2OpenBarrel();
                        ES_Timer_InitTimer(GUN_TIMER, 500);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case GUN_TIMER:
                                nextState = FIRE;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            default: break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(GUN_TIMER);
                        break;

                    default: break;
                }
            }
            break;


        case FIRE:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("FIRE!!!\n");
                        R2CloseBarrel();
                        if (firecount < 5) { //Since we aren't firing all the balls
                            dbprintf("Fire %d\n", firecount);
                            nextState = InitCannon;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                            firecount++;
                        } else {
                            dbprintf("shot opponent\n");
                            R2LauncherMotorSpeed(0);
                            ThisEvent.EventType = SHOT_OPPONENT;
                            ThisEvent.EventParam = TRUE;
                            Post_R2_BJT2_HSM(ThisEvent);
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                        break;

                    case ES_EXIT: break;

                    default: break;
                }
            }
            break;


        default: break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunR2MainCannon(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunR2MainCannon(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/


/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

#ifdef R2MAAINCANNON_TEST // <-- change this name and define it in your MPLAB-X
//     project to run the test harness
#include <stdio.h>

void main(void) {
    ES_Return_t ErrorType;
    BOARD_Init();
    // When doing testing, it is useful to annouce just which program
    // is running.

    printf("Starting the Hierarchical State Machine Test Harness \r\n");
    printf("using the 2nd Generation Events & Services Framework\n\r");

    // Your hardware initialization function calls go here

    // now initialize the Events and Services Framework and start it running
    ErrorType = ES_Initialize();

    if (ErrorType == Success) {
        ErrorType = ES_Run();
    }

    //
    //if we got to here, there was an error
    //

    switch (ErrorType) {
        case FailedPointer:
            printf("Failed on NULL pointer");
            break;
        case FailedInit:
            printf("Failed Initialization");
            break;
        default:
            printf("Other Failure");
            break;
    }

    while (1) {
        ;
    }
}

#endif // FindOpponentHSM_TEST
