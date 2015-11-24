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
#include "FindOpponentHSM.h"
#include "ObstacleFollowing.h"
#include "R2Events.h"
#include "R2_MainCannon.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_FindOpponent_STATES(STATE) \
        STATE(InitFindOpponentState)    \
        STATE(HitCenter) \
        STATE(WallRide) /*Make sure state names are unique in their hierachy*/ \
        STATE(Reverse)       \
        STATE(LookForEnemy)       \
        STATE(EliminateEnemy)  \
        STATE(Evade) \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_FindOpponent_STATES(ENUM_FORM)
} FindOpponentState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_FindOpponent_STATES(STRING_FORM)
};

//#define FINDOPPONENT_HSM_DEBUG_VERBOSE
#ifdef FINDOPPONENET_HSM_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

// Motor Speeds
#define FULL_SPEED_FORWARD 60
#define HALF_SPEED_FORWARD 30
#define QUARTER_SPEED_FORWARD 10
#define STOP 0
#define QUARTER_SPEED_BACKWARD -10
#define HALF_SPEED_BACKWARD -30
#define FULL_SPEED_BACKWARD -60

// Timers
#define HALF_SECOND 500
#define ONE_SECOND 1000 // Back up for one second, then continue turning
#define TWO_SECONDS 2000 // Back up for two seconds, then continue turning

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

static FindOpponentState_t CurrentState = InitFindOpponentState;
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitFindOpponentHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitFindOpponentHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitFindOpponentState;
    returnEvent = RunFindOpponentHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunFindOpponentHSM(ES_Event ThisEvent)
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

ES_Event RunFindOpponentHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    FindOpponentState_t nextState;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitFindOpponentState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT) {
                nextState = HitCenter;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case HitCenter:
            // This state assumes we have just collected ammo and that
            // we are still positioned directly in front of the ammo dump
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Tunring around to hit center. \n");
                        // being used for 180 degree turn.
                        ES_Timer_InitTimer(FIND_OPPONENT_TIMER, 1000);
                        R2Motors(-60, 0);
                        break;

                    case BUMPED:
                        // Transition to start wall following/looking for enemy
                        // Q: what if this comes from hitting a roach?
                        nextState = WallRide;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case BEACON_FOUND:
                        R2Motors(0, 0);
                        nextState = EliminateEnemy;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;


                    case ES_TIMEOUT:
                        // drive forward until you hit the center obstacle
                        if (ThisEvent.EventParam == FIND_OPPONENT_TIMER) { // not sure why.. but timer three is causing a timeout event here (SKETCH)
                            R2Motors(50, 50);
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(FIND_OPPONENT_TIMER);
                        break;

                    default:
                        break;
                }
            }
            break;

        case WallRide:
            // State objective: try to wall follow arround the center obstacle
            ThisEvent = RunObstacleFollowing(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;

                    case ES_EXIT:
                        break;

                    case BEACON_FOUND:
                        R2Motors(0, 0);
                        nextState = EliminateEnemy;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        break;

                    case TAPE_FOUND:
                        //nextState = Evade;
                        //makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default:
                        break;
                }
            }
            break;

            //        case Evade:
            //            if (ThisEvent.EventType != ES_NO_EVENT) {
            //                switch (ThisEvent.EventType) {
            //                    case ES_ENTRY:
            //                        R2Motors(-50, 50);
            //                        ES_Timer_InitTimer(FIND_OPPONENT_TIMER, 750);
            //                        break;
            //
            //                    case ES_EXIT:
            //                        ES_Timer_StopTimer(FIND_OPPONENT_TIMER);
            //                        break;
            //
            //                    case BEACON_FOUND:
            //                        R2Motors(0, 0);
            //                        nextState = EliminateEnemy;
            //                        makeTransition = TRUE;
            //                        ThisEvent.EventType = ES_NO_EVENT;
            //                        break;
            //
            //                    case ES_TIMEOUT:
            //                        if (ThisEvent.EventParam == FIND_OPPONENT_TIMER) {
            //                            R2Motors(50, 50);
            //                            ThisEvent.EventType = ES_NO_EVENT;
            //                        }
            //                        break;
            //
            //                    case BUMPED:
            //                        nextState = WallRide;
            //                        makeTransition = TRUE;
            //                        ThisEvent.EventType = ES_NO_EVENT;
            //                        break;
            //
            //                    case TAPE_FOUND:
            //                        nextState = Evade;
            //                        makeTransition = TRUE;
            //                        ThisEvent.EventType = ES_NO_EVENT;
            //                        break;
            //
            //                    default:
            //                        break;
            //                }
            //            }
            //            break; //end evade


        case LookForEnemy:
            // State objective: Makes 360 degree turn scanning for opponent.
            // slower turn should increase resolution...
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Looking for the enemy. \n");
                        rightR2Motor(-45); // tank turn rather slowly looking for opponent
                        leftR2Motor(45);
                        ES_Timer_InitTimer(FIND_OPPONENT_TIMER, 5000); // needs to make a 360 turn
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(FIND_OPPONENT_TIMER);
                        break;

                    case BEACON_FOUND:
                        rightR2Motor(0); // STOP!
                        leftR2Motor(0);
                        nextState = EliminateEnemy;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        // Didn't find opponent, move farther along on the wall
                        nextState = WallRide;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default:
                        break;
                }
            }
            break;

        case EliminateEnemy:
            // state objective: (drive closer to enemy?) shoot enemy
            ThisEvent = RunR2MainCannon(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Going to kill enemy. \n");
                        rightR2Motor(0);
                        leftR2Motor(0);
                        //ThisEvent = RunR2MainCannon(ThisEvent);
                        // Drive forward a little to get closer
                        //ES_Timer_InitTimer(FIND_OPPONENT_TIMER, ONE_SECOND);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT:
                        ES_Timer_StopTimer(FIND_OPPONENT_TIMER);
                        break;

                    case BEACON_LOST:
                        // Lost the beacon, try to find it again
                        // Note: might need a HadOpponent state or something along those
                        //      lines to optimize so we don't try going all the way around
                        //      the field again trying to find the opponent
                        rightR2Motor(0); // STOP!
                        leftR2Motor(0);
                        R2LauncherMotorSpeed(0);
                        nextState = LookForEnemy;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case SHOT_OPPONENT:
                        printf("I WANT TO DIE\n");
                        ThisEvent.EventType = SHOT_OPPONENT;
                        ThisEvent.EventParam = TRUE;
                        break;


                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case FIND_OPPONENT_TIMER:
                                break;
                            default:break;
                                // If we made it here, we should still have the beacon in sight
                                // and be close enough to shoot
                        }
                        break;

                    default: break;
                }
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunFindOpponentHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunFindOpponentHSM(ENTRY_EVENT); // <- rename to your own Run function
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

#ifdef FindOpponentHSM_TEST // <-- change this name and define it in your MPLAB-X
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
