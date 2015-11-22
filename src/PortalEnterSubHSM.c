/*
 * @File: EnterPortalSubHSM.c
 * @Author: Daniel Ruatta
 * @Brief: Starts when the FindPortalSubHSM detects a TRACKWIREEVENT.
 * When this event is detected, it will pivot right until a second
 * TRACKWIREEVENT is detected. This will align us parallel to the obstacle.
 * Once we are parallel to the obstacle, we will drive forward for 3 seconds
 * (UNDETERMINED), waiting for a second TRACKWIREEVENT. If it doesn't get a
 * TRACKWIREEVENT before TIMEOUT, it has left the portal, and will reverse
 * for 5 seconds and stop inside the portal. If it gets a TRACKWIREEVENT
 * during this PortalEnter stage, it will reverse for 2 seconds and stop
 * inside the portal.
 *
 * If a BUMPEVENT happens during this time, we have hit a roach, and will stop
 * for 3 seconds, waiting for it to pass by.
 *
 * */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "R2_BJT2_HSM.h"
#include "PortalEnterSubHSM.h"
#include "R2Events.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define LIST_OF_PORTAL_ENTER_STATES(STATE) \
STATE(InitPortalEnter) /* Init: Called at the start of this SubHSM */ \
STATE(PortalPivot)/* ENTER: TRACKWIREEVENT - EXIT: TRACKWIREEVENT */ \
STATE(PortalTest) /* ENTER: TRACKWIREEVENT - EXIT: ES_TIMEOUT || TRACKWIREEVENT */ \
STATE(InPortal) /* ENTER: TRACKWIREEVENT - EXIT:  ES_TIMEOUT */ \
STATE(OutsidePortal) /* ENTER: ES_TIMEOUT - EXIT: ES_TIMEOUT */ \
STATE(PortalStopped) /* ENTER: ES_TIMEOUT - EXIT: None */ \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_PORTAL_ENTER_STATES(ENUM_FORM)
} PortalEnterState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_PORTAL_ENTER_STATES(STRING_FORM)
};

//#define ENTER_PORTAL_SUB_HSM_DEBUG_VERBOSE
#ifdef ENTER_PORTAL_SUB_HSM_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

/********************** BACKUP TIMER NEEDED ***********************************/
#define PORTAL_TEST_TIMER 7 // Timer7 is available - Confirm with ES_Config before use
#define PORTAL_ROACH_TIMER 8 // Timer8 is available - Confirm with ES_Config before use

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static PortalEnterState_t CurrentState = InitPortalEnter; // <- change name to match ENUM
static uint8_t MyPriority;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * @Function InitPortalEnterSubHSM(void)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @author Daniel Ruatta, 11.21.2015 */
uint8_t InitPortalEnterSubHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitPortalEnter;
    returnEvent = RunPortalEnterSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        dbprintf("\n InitPortalEnter: Returning SUCCESS. \n");
        return TRUE;
    }
    return FALSE;
}

/*******************************************************************************
 * @Function RunPortalEnterSubHSM(void)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @author Daniel Ruatta, 11.21.2015 */
ES_Event RunPortalEnterSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE;
    PortalEnterState_t nextState;

    ES_Tattle();

    switch (CurrentState) {
        case InitPortalEnter: // Initializing SubHSM
            if (ThisEvent.EventType == ES_INIT) {
                dbprintf("\n InitPortalEnter: Exiting Init."
                        " Should enter PortalPivot. \n");
                nextState = PortalPivot;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break; // End of InitPortalEnter

            /******************************************************************
             * @State: PortalPivot
             * This state will pivot R2-BJT2 left so that he
             * aligns along the Track Wire and triggers a second
             * TRACK_WIRE_FOUND event. We will set the left motor
             * to 20 and the right motor to 0 to do this
             */
        case PortalPivot: // TRACKWIREDETECTED in higher level HSM,
            // aligning on the portal
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n PortalPivot: ES_ENTRY event. Pivoting"
                                "left to detect the track wire using the left"
                                "sensor. \n");
                        ThisEvent.EventType = ES_NO_EVENT;

                        /************* MOTOR SPEEDS NEEDED ************/

                        break;

                    case TRACK_WIRE_FOUND:
                        dbprintf("\n PortalPivot: TRACK_WIRE_FOUND event."
                                "Exiting PortalPivot and entering PortalTest. \n");
                        nextState = PortalTest;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    default: // Unhandled PortalPivot events
                        break;
                }
            }
            break; // End of PortalPivot

            /*******************************************************************
             * @State: PortalTest
             * This state will drive R2-BJT2 straight forward and
             * initialize the PORTAL_TIMER to 5 (UNTESTED) seconds.
             * It has one of two exit cases: We either get a
             * TRACK_WIRE_FOUND event, in which case we will
             * transition to InPortal, or a TIMEOUT event, in which
             * case we will transition to OutsidePortal. If we
             * receive a BUMPED event (a roach), we will stop for
             * 3 (UNTESTED) seconds before driving forward again. On EX_EXIT
             * we will stop all timers.
             */
        case PortalTest: // Aligned with the Portal, need to see which edge
            // we are on
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n PortalTest: ES_ENTRY event. Driving straight"
                                "forward for 5 seconds. \n");
                        ThisEvent.EventType = ES_NO_EVENT;

                        /*************** TIMER INIT NEEDED *******************
                        /************* DRIVE FORWARD NEEDED *******************/

                        break;

                    case ES_EXIT:
                        dbprintf("\n PortalTest: ES_EXIT event. Stopping "
                                "PORTAL_TEST_TIMER and PORTAL_ROACH_TIMER. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /************* STOP ROACH TIMER NEEDED **************/
                        break;

                    case ES_TIMEOUT: // Drove too far, Portal Lost
                        dbprintf("\n PortalTest: ES_TIMEOUT event. Exiting"
                                "PortalTest and entering OutsidePortal. \n");
                        nextState = OutsidePortal;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case TRACK_WIRE_FOUND: // Second track wire found, inside portal
                        dbprintf("\n PortalTest: TRACK_WIRE_FOUND event. Exiting"
                                "PortalTest and entering InPortal. \n");
                        nextState = InPortal;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                        /************** BUMPED NEEDS TO STOP ********/
                    case BUMPED: // Roach detected, stopping until it passes
                        dbprintf("\n PortalTest: BUMPED event. Roach "
                                "detected. Stopping R2-BJT2 for 3 seconds. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /*************** STOP TIMER INIT NEEDED ****************/
                        /****************** STOP NEEDED ************************/
                        break;

                    default: // Unhandled PortalTest events
                        break;
                }
            }
            break; // End of PortalTest

            /******************************************************************
             * @STATE: InPortal
             * @BRIEF: This state will reverse R2-BJT2 for 1 (UNTESTED) second so that
             * he enters the portal. If he receives a BUMPED event during this
             * state, we will enter PortalRoach. If we get an ES_TIMEOUT, we
             * will transition to RoachStopped so that R2-BJT2 will remain
             * in the portal until he is checked off.
             * @ENTER: TRACKWIREEVENT
             * @EXIT: BUMPED || ES_TIMEOUT
             */

        case InPortal: // R2-BJT2 is inside the portal, reverse for 1 (UNTESTED) second
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n InPortal: ES_ENTRY event. Initializing Reverse"
                                "Timer to 1 second and setting the motor "
                                "speeds to -30 / -30. \n");
                        ThisEvent.EventType = ES_NO_EVENT;

                        /*************** MOTOR SPEEDS NEEDED ******************/
                        /************ REVERSE TIMER INIT NEEDED ***************/

                        break;

                    case ES_EXIT:
                        dbprintf("\n InPortal: ES_EXIT event. Stopping "
                                "PORTAL_TEST_TIMER and PORTAL_ROACH_TIMER. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /************* STOP ROACH TIMER NEEDED **************/

                        break;

                    case BUMPED:
                        dbprintf("\n InPortal: BUMPED event. Roach detected."
                                "Stopping R2-BJT2 for 3 seconds. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /*************** STOP TIMER INIT NEEDED ****************/
                        /****************** STOP NEEDED *******************/

                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case PORTAL_TEST_TIMER:
                                dbprintf("\n InsidePortal: ES_TIMEOUT->"
                                        "PORTAL_TEST_TIMER. Transitioning"
                                        "to PortalStopped. \n");
                                nextState = PortalStopped;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;

                            case PORTAL_ROACH_TIMER:
                                dbprintf("\n InsidePortal: ES_TIMEOUT->"
                                        "PORTAL_ROACH_TIMER. Reversing until"
                                        "PORTAL_TEST_TIMER expires. \n");

                                /********** REVERSE TIMER START NEEDED **********/
                                /*************MOTOR  REVERSE NEEDED ************/

                                break;

                            default: // Unhandled ES_TIMEOUT events
                                break;
                        }
                        break; // End of ES_TIMEOUT case

                    default: //  Unhandled InPortal Events
                        break;
                }
            }
            break; // End of InPortal

            /******************************************************************
             * @State OutsidePortal
             * @Brief: This state will reverse R2-BJT2 for 7 (UNTESTED) seconds.
             * Since PortalTest drove us forward for 5 seconds and we never
             * reached a final TRACK_WIRE_FOUND event, we know that we started
             * on the outer edge of the track wire and drove away from the portal.
             * In order to compensate for this, we will reverse for 2 extra
             * seconds and end up inside of the portal. Any BUMPED events will
             * pause this
             * @Enter: ES_TIMEOUT from PortalTest
             * @Exit: BUMPED to PortalRoach || ES_TIMEOUT to RoachStopped
             */
        case OutsidePortal: // R2-BJT2 has driven forward past the portal
            // He will now reverse for 7 (UNTESTED) seconds and transition
            // to PortalStopped
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n OutsidePortal: ES_ENTRY event. Reversing"
                                "straight backward for 7 seconds. \n");
                        ThisEvent.EventType = ES_NO_EVENT;

                        /********** REVERSE TIMER INIT NEEDED *******************/
                        /************ MOTOR REVERSE NEEDED *******************/
                        break;

                    case ES_EXIT:
                        dbprintf("\n OutsidePortal: ES_EXIT event. Stopping "
                                "PORTAL_TEST_TIMER and PORTAL_ROACH_TIMER. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /************* STOP ROACH TIMER NEEDED **************/
                        break;

                    case BUMPED:
                        dbprintf("\n OutsidePortal: BUMPED event. Roach"
                                "detected. Stopping R2-BJT2 for 3 seconds. \n");

                        /************* STOP REVERSE TIMER NEEDED **************/
                        /*************** STOP TIMER INIT NEEDED ****************/
                        /****************** STOP NEEDED ************************/
                        break;

                    case ES_TIMEOUT:
                        switch (ThisEvent.EventParam) {
                            case PORTAL_TEST_TIMER:
                                dbprintf("\n OutsidePortal: ES_TIMEOUT->"
                                        "PORTAL_TEST_TIMER. Transitioning"
                                        "to PortalStopped. \n");
                                nextState = PortalStopped;
                                makeTransition = TRUE;
                                ThisEvent.EventType = ES_NO_EVENT;
                                break;

                            case PORTAL_ROACH_TIMER:
                                dbprintf("\n OutsidePortal: ES_TIMEOUT->"
                                        "PORTAL_ROACH_TIMER. Reversing until"
                                        "PORTAL_TEST_TIMER expires. \n");

                                /********** REVERSE TIMER START NEEDED **********/
                                /*************MOTOR  REVERSE NEEDED ************/
                                break;

                            default: // Unhandled ES_TIMEOUT events
                                break;
                        }
                        break; // End of ES_TIMEOUT case

                    default: // Unhandled OutsidePortal events.
                        break;
                }
            }
            break; // End of OutsidePortal

            /******************************************************************
             * @State PortalStopped
             * @Author: Daniel Ruatta, 11/21/2015, 5:46pm
             * @Brief: This state will stop R2-BJT2 permanently so that he
             * can be checked off. This state cannot be exited out of.
             * @Enter: ES_TIMEOUT->PORTAL_TEST_TIMER from OutsidePortal or
             * InPortal
             * @Exit: None
             *
             */
        case PortalStopped: // R2-BJT2 has entered the portal, and will now
            // stop permanently
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Entered PortalStopped. Bringing R2-BJT2"
                                "to a full stop. \n");

                        /****************** STOP NEEDED ************************/

                        break;

                    default: // Unhandled PortalStopped events
                        break;
                }
            }
            break;

        default: // Unhandled PortalEnter events
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) {
        RunPortalEnterSubHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunPortalEnterSubHSM(ENTRY_EVENT);
    }

    ES_Tail();
    return ThisEvent;
}
