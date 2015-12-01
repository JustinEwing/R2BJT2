/*
 * File: FindAmmoHSM.c
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
#include "FindAmmoHSM.h"
#include "FindPortalHSM.h"
#include "TapeFollowing.h"
#include "DumpFollowing.h"
#include "R2_MainCannon.h"
#include "LauncherServo.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_FindAmmo_STATES(STATE) \
STATE(InitFindAmmoState)\
STATE(SearchingForTape) \
STATE(ReversingRight)   \
STATE(ReversingLeft)    \
STATE(FollowTape)       \
STATE(Verify)           \
STATE(FollowDump)       \
STATE(AlignOnT)         \
STATE(Wait)         \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_FindAmmo_STATES(ENUM_FORM)
} FindAmmoState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_FindAmmo_STATES(STRING_FORM)
};

int ammoBumps = 0;

//#define FINDAMMO_HSM_DEBUG_VERBOSE
#ifdef FINDAMMO_HSM_DEBUG_VERBOSE
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

static FindAmmoState_t CurrentState = InitFindAmmoState;
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitFindAmmoHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitFindAmmoHSM(void) {
    ES_Event returnEvent;

    dbprintf("Entered %s\n", __FUNCTION__);

    CurrentState = InitFindAmmoState;
    returnEvent = RunFindAmmoHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunFindAmmoHSM(ES_Event ThisEvent)
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
ES_Event RunFindAmmoHSM(ES_Event ThisEvent) {
    //dbprintf("Entered %s with %s event and %s event parameter\n", __FUNCTION__, ThisEvent.EventType, ThisEvent.EventParam);

    uint8_t makeTransition = FALSE; // use to flag transition
    FindAmmoState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitFindAmmoState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT) {
                nextState = Wait;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case Wait:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(BACKUP_TIMER, 5);
                        break;

                    case ES_TIMEOUT:
                        nextState = SearchingForTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        break;

                    default: break;
                }
            }
            break; // End Wait

        case SearchingForTape:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        //R2Motors(30, 30);
                        ES_Timer_InitTimer(BACKUP_TIMER, 500);
                        break;

                    case TAPE_FOUND:
                        dbprintf("Found\n");
                        nextState = FollowTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case BUMPED:
                        switch (ThisEvent.EventParam) {
                            case LEFT_BUMPER:
                                nextState = ReversingRight;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            case RIGHT_BUMPER:
                                nextState = ReversingLeft;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                        }
                        break;

                    case ES_TIMEOUT:
                        R2Motors(30, 30);
                        ThisEvent.EventType = ES_NO_EVENT;
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    case ES_EXIT:
                        break;

                    default: break;
                }
            }
            break; // End Searching

        case ReversingRight:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2Motors(-30, -60);
                        ES_Timer_InitTimer(BACKUP_TIMER, 500);
                        break;


                    case ES_TIMEOUT:
                        nextState = SearchingForTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    default: break;
                }
            }
            break; //End ReversingRight

        case ReversingLeft:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2Motors(-60, -30);
                        ES_Timer_InitTimer(BACKUP_TIMER, 500);
                        break;


                    case ES_TIMEOUT:
                        nextState = SearchingForTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    default: break;
                }
            }
            break; //End ReversingLeft

        case FollowTape:
            ThisEvent = RunTapeFollowing(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case OH_SHIIIIIIIIIT:
                                nextState = SearchingForTape;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                ES_Timer_StopTimer(OH_SHIIIIIIIIIT);
                                break;
                            default:break;
                        }
                        break;

                    case BUMPED:
                        nextState = Verify;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ResetTapeFollowing();
                        break;

                    default: break;
                }
            }
            break; //End FollowTape

        case Verify: // not sure how we are doing this yet...
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        R2FullStop();
                        ES_Timer_InitTimer(BACKUP_TIMER, 2000);
                        break;

                    case UNBUMPED:
                        nextState = FollowTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        nextState = FollowDump;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        break;

                    default: break;
                }
            }
            break; //End Verify

        case FollowDump:
            ThisEvent = RunDumpFollowing(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(OH_SHIIIIIIIIIT, 8000); // going to try 7 seconds...
                        break;

                    case BUMPED:
                        break;

                    case ES_TIMEOUT:
                        nextState = SearchingForTape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_LOST:
                        switch (ThisEvent.EventParam) {
                            case TOP_TAPE_SENSOR:
                                nextState = AlignOnT;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;
                            default:break;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(OH_SHIIIIIIIIIT);
                        ResetDumpFollowing();
                        break;

                    default: break;
                }
            }
            break; //End FollowDump

        case AlignOnT:
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        ES_Timer_InitTimer(OH_SHIIIIIIIIIT, 10000);
                        R2Motors(-10, -20);
                        break;

                    case BUMPED:
                        R2FullStop();
                        ES_Timer_InitTimer(BACKUP_TIMER, 2000);
                        ES_Timer_StopTimer(OH_SHIIIIIIIIIT);
                        break;

                    case UNBUMPED:
                        R2Motors(-10, -30);
                        ES_Timer_StopTimer(BACKUP_TIMER);
                        ES_Timer_InitTimer(OH_SHIIIIIIIIIT, 6000);
                        break;

                    case TAPE_FOUND:
                        switch (ThisEvent.EventParam) {
                            case TOP_TAPE_SENSOR:
                                R2Motors(30, 20);
                                break;
                        }
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TAPE_LOST:
                        switch (ThisEvent.EventParam) {
                            case TOP_TAPE_SENSOR:
                                R2Motors(-10, -30);
                                break;
                            default:break;
                        }
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case BACKUP_TIMER:
                                ES_Timer_StopTimer(BACKUP_TIMER);
                                ES_Timer_StopTimer(OH_SHIIIIIIIIIT); // have to stop other timer too!!!!
                                ThisEvent.EventType = FOUND_AMMO;
                                ThisEvent.EventParam = TRUE;
                                break;

                            case OH_SHIIIIIIIIIT:
                                nextState = SearchingForTape;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                ES_Timer_StopTimer(OH_SHIIIIIIIIIT);
                                break;
                            default:break;
                        }
                        break;

                    default: break;
                }
            }
            break; //End AlignOnT



        default: break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunFindAmmoHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunFindAmmoHSM(ENTRY_EVENT);
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

#ifdef FindAmmoHSM_TEST // <-- change this name and define it in your MPLAB-X
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

#endif // FindAmmoHSM_TEST
