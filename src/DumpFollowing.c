/*
 * File: DumpFollowing.c
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
#include "DumpFollowing.h"
#include "FindAmmoHSM.h"
#include "R2TapeEvents.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_DUMP_FOLLOWING_STATES(STATE)    \
        STATE(InitDumpFollowingState)           \
        STATE(Reverse)                          \
        STATE(TankLeft)                         \
        STATE(TurnRight)                        \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_DUMP_FOLLOWING_STATES(ENUM_FORM)
} DumpFollowingState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_DUMP_FOLLOWING_STATES(STRING_FORM)
};


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

static DumpFollowingState_t CurrentState = InitDumpFollowingState; // <- change name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitDumpFollowing(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitDumpFollowing(void) {
    ES_Event returnEvent;

    CurrentState = InitDumpFollowingState;
    returnEvent = RunDumpFollowing(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunDumpFollowing(ES_Event ThisEvent)
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
ES_Event RunDumpFollowing(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    DumpFollowingState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitDumpFollowingState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                nextState = Reverse;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case Reverse: // with slight left to stay in bounds
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2Motors(-30, -50);
                        ES_Timer_InitTimer(BACKUP_TIMER, 400);
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    case ES_TIMEOUT:
                        nextState = TankLeft;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_FOUND:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_LOST:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break; //End Reverse

        case TankLeft:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2Motors(-40, 50);
                        ES_Timer_InitTimer(BACKUP_TIMER, 350);
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    case ES_TIMEOUT:
                        nextState = TurnRight;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_FOUND:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                        case TAPE_LOST:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;


                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break; //End TankLeft

        case TurnRight:
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2Motors(50, 25);
                        break;

                    case ES_EXIT:
                        break;

                    case BUMPED:
                        nextState = Reverse;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break; //End TurnRight

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunDumpFollowing(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunDumpFollowing(ENTRY_EVENT); // <- rename to your own Run function
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

#ifdef DumpFollowing_TEST // <-- change this name and define it in your MPLAB-X
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

#endif // DumpFollowing_TEST
