/*
 * File: R2Service.c
 * @Author Daniel Ruatta, 11/13/2015 7:00pm
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "R2_BJT2_HSM.h"
#include "R2BumperService.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define R2_BUMPER_TIMER 4
#define FIVE_MILLISECONDS 5

// Need 4 checking masks
#define FIRST_BIT (0x1)
#define SECOND_BIT (0x2)
#define SHIFT_LEFT_ONE 1

// Also need a final checking mask to see if a bumper is pressed by a human
#define ALL_BITS (0xff)
#define ZERO_BITS (0x00)

// Bumper Pins (Pins Y3 and Y4) - These are digital I/O pins (1 or 0)
#define LEFT_BUMPER_INIT PORTY03_TRIS // Pin Y03 I/O
#define RIGHT_BUMPER_INIT PORTY04_TRIS // Pin Y04 I/O

// Initialization
#define INPUT 1


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */




/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static bumpstate_t PreviousFLState = WAS_UNBUMPED;
static bumpstate_t PreviousFRState = WAS_UNBUMPED;

static uint8_t MyPriority;

// Endianness is Last Bit<->First Bit: Ex) Last: 100001 First
unsigned char FrontLeftBumper = 0; // First Bit
unsigned char FrontRightBumper = 0; // Second Bit

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitR2Service(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunR2Service function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitR2BumperService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // Initialize Timers
    ES_Timer_Init();


    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostR2Service(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostR2BumperService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunR2BumperService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the service,
 *        as this is called any time a new event is passed to the event queue. 
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed." 
 * @author J. Edward Carryer, 2011.10.23 19:25 */
ES_Event RunR2BumperService(ES_Event ThisEvent) {
    // This service is supposed to run because of a timeout event!
    // Posts events whenever the bumper's status changes
    
    ES_Event ReturnEvent;
    ES_Event BumpEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    if (ThisEvent.EventType == ES_TIMEOUT) {
        uint8_t bump_state = CheckBumpers();

        // Debugging printf
        //printf("After Bit Test: %x\n", (bump_state & SECOND_BIT));

        // Left Bumper Buffer Reading
        FrontLeftBumper <<= SHIFT_LEFT_ONE;
        FrontLeftBumper |= ((bump_state & SECOND_BIT)>>1);
        //printf("FrontLeftBumper: %x\n", FrontLeftBumper);
        // Left Bumper Buffer Reading
        FrontRightBumper <<= SHIFT_LEFT_ONE;
        FrontRightBumper |= (bump_state & FIRST_BIT);

        // Left Bumper Test - BUMPED check
        if ((FrontLeftBumper == ALL_BITS)
                && (PreviousFLState == WAS_UNBUMPED)) {
            BumpEvent.EventType = BUMPED;
            //BumpEvent.EventParam = (uint16_t) bump_state;
            BumpEvent.EventParam = LEFT_BUMPER;
            Post_R2_BJT2_HSM(BumpEvent);
            PreviousFLState = WAS_BUMPED;
        }            // Left Bumper Test - UNBUMPED check
        else if ((FrontLeftBumper == ZERO_BITS)
                && (PreviousFLState == WAS_BUMPED)) {
            BumpEvent.EventType = UNBUMPED;
            //BumpEvent.EventParam = (uint16_t) bump_state;
            BumpEvent.EventParam = LEFT_BUMPER;
            Post_R2_BJT2_HSM(BumpEvent);
            PreviousFLState = WAS_UNBUMPED;
        }

        // Right Bumper Test - BUMPED check
        if ((FrontRightBumper == ALL_BITS)
                && (PreviousFRState == WAS_UNBUMPED)) {
            BumpEvent.EventType = BUMPED;
            //BumpEvent.EventParam = (uint16_t) bump_state;
            BumpEvent.EventParam = RIGHT_BUMPER;
            Post_R2_BJT2_HSM(BumpEvent);
            PreviousFRState = WAS_BUMPED;
        }
            // Right Bumper Test - UNBUMPED check
        else if ((FrontRightBumper == ZERO_BITS)
                && (PreviousFRState == WAS_BUMPED)) {
            BumpEvent.EventType = UNBUMPED;
            //BumpEvent.EventParam = (uint16_t) bump_state;
            BumpEvent.EventParam = RIGHT_BUMPER;
            Post_R2_BJT2_HSM(BumpEvent);
            PreviousFRState = WAS_UNBUMPED;
        }

        // Reset the Timer
        // Initialize R2BumperService Timer to 5 Milliseconds
        ES_Timer_InitTimer(R2_BUMPER_TIMER, FIVE_MILLISECONDS); // Set Timer4 to 5 milliseconds

    }

    if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
    {
        // Initialize R2BumperService Timer to 5 Milliseconds
        ES_Timer_InitTimer(R2_BUMPER_TIMER, FIVE_MILLISECONDS); // Set Timer4 to 5 milliseconds
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/

/******************************************************************
 * @Function InitBumpers
 * @Param void
 * @Return SUCCESS or ERROR
 * @Author Daniel Ruatta, 11/13/2015
 * @Brief Sets the Bumper pins (Y3 and Y4) to inputs so that we
 * can read BUMP_EVENTS from them
 * @Usage To be called in main() to initialize our bumpers
 */
uint8_t InitBumpers(void) {
    return IO_PortsSetPortInputs((LEFT_BUMPER_INIT | RIGHT_BUMPER_INIT), INPUT);
}

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

#ifdef R2SERVICE_TEST // <-- change this name and define it in your MPLAB-X
//     project to run the test harness
#include <stdio.h>

void main(void) {
    ES_Return_t ErrorType;
    BOARD_Init();
    // When doing testing, it is useful to annouce just which program
    // is running.

    printf("Starting the Simple Service Test Harness \r\n");
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

#endif // R2SERVICE_TEST