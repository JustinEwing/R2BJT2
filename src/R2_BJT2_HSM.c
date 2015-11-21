/*
 * File: R2_BJT2_HSM.c
 * Author: Brushless DC-3PO
 *
 * File to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class.
 *
 * There is another template file for the SubHSM's that is slightly differet, and
 * should be used for all of the subordinate state machines (flat or heirarchical)
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 11/11/15       DTL     Started writing HSM/because you gotta start somewhere
 *
 *
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "R2_BJT2_HSM.h"
#include "FindAmmoHSM.h"
#include "FindOpponentHSM.h"
#include "FindPortalHSM.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//Include any defines you need to do
/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_HSM_STATES(STRING_FORM)
};

//#define HSM_DEBUG_VERBOSE
#ifdef HSM_DEBUG_VERBOSE
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
   relevant to the behavior of this state machine
   Example: char RunAway(uint_8 seconds);*/
/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static R2_BJT2_State_t CurrentState = InitHSMtate; // <- change enum name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function Init_R2_BJT2_HSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside Run_R2_BJT2_FSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t Init_R2_BJT2_HSM(uint8_t Priority) {
    dbprintf("Entered %s\n", __FUNCTION__);
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = InitHSMtate;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function Post_R2_BJT2_HSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t Post_R2_BJT2_HSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function Query_R2_BJT2_HSM(void)
 * @param none
 * @return Current state of the state machine
 * @brief This function is a wrapper to return the current state of the state
 *        machine. Return will match the ENUM above. Remember to rename to
 *        something appropriate, and also to rename the R2_BJT2_State_t to your
 *        correct variable name.
 * @author J. Edward Carryer, 2011.10.23 19:25 */
R2_BJT2_State_t Query_R2_BJT2_HSM(void) {
    return (CurrentState);
}

/**
 * @Function Run_R2_BJT2_HSM(ES_Event ThisEvent)
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
ES_Event Run_R2_BJT2_HSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    R2_BJT2_State_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitHSMtate: // If current state is initial Pseudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state
                // Initialize all sub-state machines
                dbprintf("Entered %s\n", __FUNCTION__);

                InitFindAmmoHSM();
                InitTapeFollowing();
                InitDumpFollowing();
                InitFindOpponentHSM();
                InitObstacleFollowing();
                //InitFindPortalHSM();

                // now put the machine into the actual initial state
                nextState = FindAmmo; // Should be FindAmmo
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case FindAmmo: // in the first state, replace this with correct names

            ThisEvent = RunFindAmmoHSM(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is still active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("Entered %s\n", __FUNCTION__);
                        break;

                    case ES_EXIT:
                        break;

                    case ES_KEYINPUT:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case FOUND_AMMO:
                        nextState = FindOpponent;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default:
                        break;
                }
            }
            break;

        case FindOpponent:
            ThisEvent = RunFindOpponentHSM(ThisEvent);
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        break;

                    case ES_EXIT:
                        break;

                    case ES_KEYINPUT:
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_TIMEOUT:
                        break;

                    default:
                        break;
                }
            }
            break;

        case FindPortal:
            ThisEvent = RunFindPortalHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;

                case ES_EXIT:
                    break;

                case ES_KEYINPUT:
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_TIMEOUT:
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                default:
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        Run_R2_BJT2_HSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        Run_R2_BJT2_HSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
/*Here's where you put the actual content of your functions.
Example:
 * char RunAway(uint_8 seconds) {
 * Lots of code here
 * } */

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
/* Define R2_BJT2_FSM_TEST to run this file as your main file (without the rest
 * of the framework)-useful for debugging */
#ifdef R2_BJT2_HSM_TEST // <-- change this name and define it in your MPLAB-X
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

#endif // R2_BJT2_HSM_TEST
