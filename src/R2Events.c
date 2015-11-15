#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "R2Events.h"

#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "R2_BJT2_HSM.h"
#include "IO_Ports.h"


/******************************* Private #defines ****************************/


#define LEFT_BUMPER_READ PORTY03_BIT // Pin Y03 Read
#define RIGHT_BUMPER_READ PORTY04_BIT // Pin Y04 Read


/*******************************************************************
 * @Function CheckBumps
 * @Param void
 * @Return 2-bit value representing the bumpers in following order:
 *  1. FrontLeft
 *  2. FrontRight
 * @Author Daniel Ruatta, 11/13/2015
 * @Brief Returns the state of the two bumpers:
 * 1. Create a uint8_t to store their combined state
 * 2. Use IO_PortsReadPort to read the digital value of the left bumper
 * 3. Use |= to SET that value as the first bit in bump_state
 * 4. Bit shift that value into the second bit in bump_state
 * 5. Use IO_PortsReadPort to read the digital value of the right bumper
 * 6. Use |= to SET that value as the first bit in bump_state
 * @Usage To be called within BumperService
 */
uint8_t CheckBumpers(void) {
    //      1. Create a uint8_t to store their combined state
    uint8_t BumperStates;

    //  2. Use IO_PortsReadPort to read the digital value of the left bumper
    //  3. Use |= to SET that value as the first bit in bump_state
    BumperStates = LEFT_BUMPER_READ;
    //  4. Bit shift that value into the second bit in bump_state
    BumperStates <<= 1;

    //  5. Use IO_PortsReadPort to read the digital value of the right bumper
    //  6. Use |= to SET that value as the first bit in bump_state
    BumperStates |= RIGHT_BUMPER_READ;

    return BumperStates; // temp
}


uint8_t CheckTape(void){

    /***************** Declarations ****************/
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;

    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    thisEvent = CheckTapeReading();

    //If an event has happened.
    if(thisEvent.EventType != ES_NO_EVENT) {
        returnVal = TRUE;
    }

    return returnVal;
}