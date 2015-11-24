

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "R2_BJT2_HSM.h"
#include "FindPortalHSM.h"
#include "ObstacleFollowing.h"
#include "PortalEnterSubHSM.h"
#include "R2Events.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_FindPortal_STATES(STATE) \
        STATE(InitFindPortal) /* Init: Called at the start of this SubHSM */ \
        STATE(ObstacleFollow) /* ObstacleFollow: Has its own SubHSM*/ \
        STATE(PortalEnter) /* PortalEnter: Has its own SubHSM */\

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
 * @author Daniel Ruatta, 2015.11.21 00:18 */
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
 * @author Daniel Ruatta, 2015.11.21 00:18 */
ES_Event RunFindPortalHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    FindPortalState_t nextState; // <- change type to correct enum

    ES_Tattle();

    switch (CurrentState) {
        case InitFindPortal:
            if (ThisEvent.EventType == ES_INIT) {
                dbprintf("\n. Exiting Init. Should enter ObstacleFollow."
                        "Also initializing PortalEnterSubHSM and"
                        "ObstacleFollowingSubHSM. \n");
                nextState = ObstacleFollow;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case ObstacleFollow:
            RunObstacleFollowing(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n. Entered ObstacleFollow. \n");
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TRACK_WIRE_FOUND:
                        dbprintf("\n Exiting ObstacleFollow: TRACK_WIRE_FOUND."
                                " Should enter PortalEnter. \n");

                        nextState = PortalEnter;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case PortalEnter:
            //ThisEvent = RunPortalEnterSubHSM(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Entered PortalEnter. \n");
                        R2Motors(0,0);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
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
