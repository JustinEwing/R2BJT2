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
STATE(PortalTest) /* ENTER: TRACKWIREEVENT - EXIT: TIMEOUT || TRACKWIREEVENT || BUMPED */ \
STATE(InPortal) /* ENTER: TRACKWIREEVENT - EXIT: BUMPED */ \
STATE(OutsidePortal) /* ENTER: TIMEOUT EXIT: BUMPED */ \

#define ENUM_FORM(STATE) STATE, //Enums are reprinted verbatim and comma'd

typedef enum {
    LIST_OF_PORTAL_ENTER_STATES(ENUM_FORM)
} PortalEnterState_t;

#define STRING_FORM(STATE) #STATE, //Strings are stringified and comma'd
static const char *StateNames[] = {
    LIST_OF_PORTAL_ENTER_STATES(STRING_FORM)
};

#define ENTER_PORTAL_SUB_HSM_DEBUG_VERBOSE
#ifdef ENTER_PORTAL_SUB_HSM_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

/********************** BACKUP TIMER NEEDED ***********************************/

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
            break;

        case PortalPivot: // TRACKWIREDETECTED in higher level HSM,
            // aligning on the portal
            if (ThisEvent.EventType != ES_NO_EVENT) {
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        dbprintf("\n Entered PortalPivot. \n");
                        ThisEvent.EventType = ES_NO_EVENT;
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
            break;

        case PortalTest:
            break;

        case InPortal:
            break;

        case OutsidePortal:
            break;

            if (makeTransition == TRUE) {
                RunPortalEnterSubHSM(EXIT_EVENT);
                CurrentState = nextState;
                RunPortalEnterSubHSM(ENTRY_EVENT);
            }

            ES_Tail();
            return ThisEvent;
    }
}