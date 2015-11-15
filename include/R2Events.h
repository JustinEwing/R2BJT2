/* 
 * File:   R2Events.h
 * Author: Brushless DC-3PO
 *
 * Created on January 3, 2013, 4:53 PM
 */

#ifndef R2_EVENTS_H
#define	R2_EVENTS_H

#include "AD.h"

// Tape Sensors
#define TOP_TAPE_SENSOR       (1<<0) // Should be PortV5
#define LEFT_TAPE_SENSOR      (1<<1)
#define RIGHT_TAPE_SENSOR     (1<<2)
#define LAUNCHER_TAPE_SENSOR  (1<<3)

/*
 * File:   TapeSensorFrameworkEvents.h
 * Author: Daniel Ruatta
 *
 * Created on 11/11/2015
 *
 * Tape Sensors will be read in one at a time in their corresponding
 * CheckTape functions (example: CheckTopTape). Each of these
 * functions will use Hysteresis thresholds and previous Tape States
 * to determine if tape has been lost (TAPE_LOST) or found
 * (TAPE_FOUND). If so, it will post these events, along with an
 * integer param (example: TOP_TAPE_SENSOR) to the framework. There
 * will be four tape checking events.
 *
 * Note that these event checkers, like all others,
 * require Init code. This is located at the bottom of
 * the .c file, and should be called in main().
 */

/**************************************************************************
 * @Function CheckTopTape
 * @Param void
 * @Return uint8_t so that the ES_Framework can call it
 * @Author Daniel Ruatta, 11.12/2015
 * @Brief
 * 1. Gets a Tape Reading for the Top Tape Sensor
 * 2. Checks if this reading has passed its next
 * Hysteresis Threshold
 * 3. If so, post an event to the TapeSensorHSM
 * 4. Return TRUE if an event has been posted, FALSE otherwise
 */
uint8_t CheckTopTape(void);

/**************************************************************************
 * @Function CheckLeftTape
 * @Param void
 * @Return uint8_t so that the ES_Framework can call it
 * @Author Daniel Ruatta, 11.12/2015
 * @Brief
 * 1. Gets a Tape Reading for the Left Tape Sensor
 * 2. Checks if this reading has passed its next
 * Hysteresis Threshold
 * 3. If so, post an event to the TapeSensorHSM
 * 4. Return TRUE if an event has been posted, FALSE otherwise
 */
uint8_t CheckLeftTape(void);

/***************************************************************************
 * @Function CheckRightTape
 * @Param void
 * @Return uint8_t so that the ES_Framework can call it
 * @Author Daniel Ruatta, 11.12/2015
 * @Brief
 * 1. Gets a Tape Reading for the Right Tape Sensor
 * 2. Checks if this reading has passed its next
 * Hysteresis Threshold
 * 3. If so, post an event to the TapeSensorHSM
 * 4. Return TRUE if an event has been posted, FALSE otherwise
 */
uint8_t CheckRightTape(void);

/**************************************************************************
 * @Function CheckLauncherTape
 * @Param void
 * @Return uint8_t so that the ES_Framework can call it
 * @Author Daniel Ruatta, 11.12/2015
 * @Brief
 * 1. Gets a Tape Reading for the Launcher Tape Sensor
 * 2. Checks if this reading has passed its next
 * Hysteresis Threshold
 * 3. If so, post an event to the TapeSensorHSM
 * 4. Return TRUE if an event has been posted, FALSE otherwise
 */
uint8_t CheckLauncherTape(void);

/*************************************************************
 * @Function InitTape()
 * @Param void
 * @Return uint8_t
 * @Author Daniel Ruatta, 11.09.2015
 * @Brief
 * - Calls AD_AddPins on the 4 Tape Sensor pins.
 * - Should return SUCCESS, assuming the IO_Ports were all set to INPUTS.
 * - Will return ERROR if any of the IO_Ports were not set to INPUTS.
 * @Usage
 * - To be called with the rest of our hardware initializations
 */

uint8_t InitTape();

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

#endif	/* ROACHFRAMEWORKEVENTS_H */

