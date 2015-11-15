#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "R2Events.h"

#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "R2_BJT2_HSM.h"
#include "IO_Ports.h"
#include "AD.h"



/************************** #DEFINES **********************
 * - 4 Tape sensors, so we need 4 ports for our tape sensors
 *  - Using PortV5 for Sensor A
 *  - Using PortV6 for Sensor B
 *  - Using PortV7 for Sensor C
 *  - Using PortV8 for Ball Launcher Sensor
 *
 */

#define DELAY(x)         for (wait = 0; wait <= x; wait++) {asm("nop");}
unsigned int wait = 0;

//DEFINED IN H ALSO
//// Tape Sensors
#define TOP_TAPE_PIN       AD_PORTV8 // Should be PortV5
#define LEFT_TAPE_PIN      AD_PORTV6
#define RIGHT_TAPE_PIN     AD_PORTV7
#define LAUNCHER_TAPE_PIN  AD_PORTV8 // Needs to be changed


#define TEST_TAPE_SENSOR      TOP_TAPE_PIN

// Analog to Digital Conversion
#define BLACK_HYSTERESIS 900 // DIGITAL_ONE
#define WHITE_HYSTERESIS 800 // DIGITAL_ZERO

// Hysteresis Cap
#define ROOM_LIGHT_LEVEL 1023

/******************************* #defines ****************************/


#define LEFT_BUMPER_READ PORTY03_BIT // Pin Y03 Read
#define RIGHT_BUMPER_READ PORTY04_BIT // Pin Y04 Read


// Bit shifting
#define SHIFT_LEFT_ONE 1


typedef enum {
    BLACK_TO_WHITE, WHITE_TO_BLACK
} tapestate_t;

/* @Function CheckTopTape
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
uint8_t CheckTopTape(void) {
    /***************** Declarations ****************/
    // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevTapeState = BLACK_TO_WHITE;

    // The New Tape State, which should be initialized to the current PrevTapeState
    // That way, its value will update at the beginning of every Check()
    tapestate_t NewTapeState = PrevTapeState; // Temp value

    // TapeReading, to be given a value with ReadTape()
    uint16_t TapeReading = 0; // NEEDS TO BE UINT16_T to fit an ADC value
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    // Read Top Tape Sensor
    TapeReading = AD_ReadADPin(TOP_TAPE_PIN);

    // Debugging printf
    //DELAY(100000);
    //printf("\n TapeReading = %d \n", TapeReading);
    
    // Case: Tape Found
    if ((PrevTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = TOP_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = WHITE_TO_BLACK;
        returnVal = TRUE;

    }

    // Case: Tape Lost
    if ((PrevTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = TOP_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = BLACK_TO_WHITE;
        returnVal = TRUE;
    }

    // PrevTapeState updated with NewTapeState
    PrevTapeState = NewTapeState;
    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return returnVal;

}

/* @Function CheckLeftTape
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
uint8_t CheckLeftTape(void) {
    /***************** Declarations ****************/
    // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevTapeState = BLACK_TO_WHITE;

    // The New Tape State, which should be initialized to the current PrevTapeState
    // That way, its value will update at the beginning of every Check()
    tapestate_t NewTapeState = PrevTapeState; // Temp value

    // TapeReading, to be given a value with ReadTape()
    uint16_t TapeReading;
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    // Read Left Tape Sensor
    TapeReading = AD_ReadADPin(LEFT_TAPE_PIN);

    // Case: Tape Found
    if ((PrevTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = LEFT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = WHITE_TO_BLACK;
        returnVal = TRUE;

    }

    // Case: Tape Lost
    if ((PrevTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = LEFT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = BLACK_TO_WHITE;
        returnVal = TRUE;
    }

    // PrevTapeState updated with NewTapeState
    PrevTapeState = NewTapeState;
    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return returnVal;
}

/* @Function CheckRightTape
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
uint8_t CheckRightTape(void) {
    /***************** Declarations ****************/
    // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevTapeState = BLACK_TO_WHITE;

    // The New Tape State, which should be initialized to the current PrevTapeState
    // That way, its value will update at the beginning of every Check()
    tapestate_t NewTapeState = PrevTapeState; // Temp value

    // TapeReading, to be given a value with ReadTape()
    uint16_t TapeReading;
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    // Read Left Tape Sensor
    TapeReading = AD_ReadADPin(RIGHT_TAPE_PIN);

    // Case: Tape Found
    if ((PrevTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = RIGHT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = WHITE_TO_BLACK;
        returnVal = TRUE;

    }

    // Case: Tape Lost
    if ((PrevTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = RIGHT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = BLACK_TO_WHITE;
        returnVal = TRUE;
    }

    // PrevTapeState updated with NewTapeState
    PrevTapeState = NewTapeState;
    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return returnVal;
}

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
uint8_t CheckLauncherTape(void) {
    /***************** Declarations ****************/
    // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevTapeState = BLACK_TO_WHITE;

    // The New Tape State, which should be initialized to the current PrevTapeState
    // That way, its value will update at the beginning of every Check()
    tapestate_t NewTapeState = PrevTapeState; // Temp value

    // TapeReading, to be given a value with ReadTape()
    uint16_t TapeReading;
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    // Read Left Tape Sensor
    TapeReading = AD_ReadADPin(LAUNCHER_TAPE_PIN);

    // Case: Tape Found
    if ((PrevTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = LAUNCHER_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = WHITE_TO_BLACK;
        returnVal = TRUE;

    }

    // Case: Tape Lost
    if ((PrevTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = LAUNCHER_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTapeState = BLACK_TO_WHITE;
        returnVal = TRUE;
    }

    // PrevTapeState updated with NewTapeState
    PrevTapeState = NewTapeState;
    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return returnVal;
}

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

uint8_t InitTape(void) {
    return AD_AddPins(TOP_TAPE_PIN |
            LEFT_TAPE_PIN |
            RIGHT_TAPE_PIN |
            LAUNCHER_TAPE_PIN);
}

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
    BumperStates |= IO_PortsReadPort(LEFT_BUMPER_READ);
    //  4. Bit shift that value into the second bit in bump_state
    BumperStates <<= SHIFT_LEFT_ONE;

    //  5. Use IO_PortsReadPort to read the digital value of the right bumper
    //  6. Use |= to SET that value as the first bit in bump_state
    BumperStates |= IO_PortsReadPort(RIGHT_BUMPER_READ);

    return BumperStates; // temp
}