/* 
 * File:   R2Events.h
 * Author: Brushless DC-3PO
 *
 * Created on January 3, 2013, 4:53 PM
 */

#ifndef R2_EVENTS_H
#define	R2_EVENTS_H

#include "TrackWireEvents.h"
#include "R2TapeEvents.h"


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
uint8_t CheckBumpers(void);


uint8_t CheckTape(void);

uint8_t CheckTrackWire(void);

uint8_t CheckBeacon(void);

#endif	/* R2EVENTS_H */

