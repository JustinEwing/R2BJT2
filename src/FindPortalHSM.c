/*
 * File: FindPortalHSM.c
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
#include "FindPortalHSM.h"
#include "R2Events.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_FindPortal_STATES(STATE) \
        STATE(InitFindPortal)  \
        STATE(PortalTurnLeft)  \
        STATE(PortalBackUp)    \
        STATE(EnterPortal)     \
        STATE(PortalStop)      \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_FindPortal_STATES(ENUM_FORM)
} FindPortalState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_FindPortal_STATES(STRING_FORM)
};

//#define FINDPORTAL_HSM_DEBUG_VERBOSE
#ifdef FINDPORTAL_HSM_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

// Timers
#define BACKUP_TIMER 3 // Timer3 - Confirm with ES_Config before use
#define FIND_PORTAL_TIMER 5 // Timer5 - Confirm with ES_Config before use
#define HALF_SECOND 500
#define ONE_SECOND 1000 // Back up for one second, then continue turning
#define TWO_SECONDS 2000 // Back up for two seconds, then continue turning
#define FIVE_SECONDS 5000 // Five seconds, for testing spinning bot after FOUND_PORTAL

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

static FindPortalState_t CurrentState = InitFindPortal; // <- change name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitFindPortalHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitFindPortalHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitFindPortal;
    returnEvent = RunFindPortalHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunFindPortalHSM(ES_Event ThisEvent)
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
ES_Event RunFindPortalHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    FindPortalState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitFindPortal:
            if (ThisEvent.EventType == ES_INIT) {
                dbprintf("\n. Exiting Init. Should enter TurnLeft. \n");

                nextState = PortalTurnLeft; // For testing. Normally is TurnLeft
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case PortalTurnLeft:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n. Entered TurnLeft. \n");

                        // NEED TO SET MOTOR SPEEDS
                        rightR2Motor(40); // for testing
                        leftR2Motor(25); // for testing
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;


                    case TRACK_WIRE_FOUND:
                        dbprintf("\n Exiting TurnLeft: TRACK_WIRE_FOUND."
                                " Should enter EnterPortal. \n");

                        nextState = EnterPortal;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case BUMPED:
                        dbprintf("\n Exiting TurnLeft: BUMPED."
                                " Should enter BackUp. \n");

                        nextState = PortalBackUp;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_FOUND:
                        dbprintf("\n Exiting TurnLeft: TAPE_FOUND."
                                " Should enter BackUp. \n");

                        nextState = PortalBackUp;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                        //                case ES_TIMEOUT:
                        //                     create the case statement for all other events that you are
                        //                     interested in responding to.
                        //                    nextState = BackUp;
                        //                    makeTransition = TRUE;
                        //                    ThisEvent.EventType = ES_NO_EVENT;
                        //                    break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case PortalBackUp:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n. Entered Backup. "
                                "Intializing BACKUP_TIMER.\n");

                        // NEED TO SET MOTOR SPEEDS
                        rightR2Motor(-5); // for testing
                        leftR2Motor(-5); // for testing

                        ES_Timer_InitTimer(BACKUP_TIMER, FIVE_SECONDS); // for testing.
                        // Should be HALF_SECOND

                        ThisEvent.EventType = ES_NO_EVENT;
                        break;


                    case TRACK_WIRE_FOUND:
                        dbprintf("\n Exiting PortalBackUp: TRACK_WIRE_FOUND."
                                " Should enter EnterPortal. \n");

                        nextState = EnterPortal;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                        // Commented out for testing
                    case ES_TIMEOUT:
                        dbprintf("\n Backup: ES_TIMEOUT."
                                "Should enter TurnLeft. \n");

                        nextState = PortalTurnLeft;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case EnterPortal:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    dbprintf("\n Entered EnterPortal. \n");
                    // NEED TO SET MOTOR SPEEDS
                    // Same as TurnLeft, but spinning for testing
                    rightR2Motor(30); // for testing
                    leftR2Motor(0); // for testing

                    // ALSO NEED A TIMER
                    ES_Timer_InitTimer(FIND_PORTAL_TIMER, FIVE_SECONDS);
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                    /* Case ES_EXIT for debugging */
                    //                case ES_EXIT:
                    //                    // this is where you would put any actions associated with the
                    //                    // exit from this state
                    //
                    //                    //Debugging printf
                    //                    printf("\n EnterPortal: ES_EXIT. \n");
                    //                    break;

                case ES_TIMEOUT: // Currently the FOUND paramenter is an ES_TIMEOUT
                    // create the case statement for all other events that you are
                    // interested in responding to. This one does a transition

                    dbprintf("\n EnterPortal: ES_TIMEOUT."
                            "Should enter Stop. \n");

                    nextState = PortalStop;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        case PortalStop:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    dbprintf("\n Entered PortalStop. \n");

                    // NEED TO SET MOTOR SPEEDS TO ZERO
                    rightR2Motor(0); // for testing
                    leftR2Motor(0); // for testing
                    break;

                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;


        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunFindPortalHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunFindPortalHSM(ENTRY_EVENT);
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

#ifdef FindPortalHSM_TEST // <-- change this name and define it in your MPLAB-X
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

#endif // FindPortalHSM_TEST
