/* 
 * File:   R2_TapeEvents.c
 * Author: jpewing
 *
 * Created on November 14, 2015, 9:18 PM
 */

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "AD.h"
#include "R2TapeEvents.h"

/************************** #DEFINES **********************
 * - 4 Tape sensors, so we need 4 ports for our tape sensors
 *  - Using PortV5 for Sensor A
 *  - Using PortV6 for Sensor B
 *  - Using PortV7 for Sensor C
 *  - Using PortV8 for Ball Launcher Sensor
 *
 */

#define DELAY(x)         for (wait = 0; wait <= x; wait++) {asm("nop");} // <-not supposed to have semicolon??
unsigned int wait = 0;
//// Tape Sensors
#define TOP_TAPE_PIN       AD_PORTV8 
#define LEFT_TAPE_PIN      AD_PORTV6
#define RIGHT_TAPE_PIN     AD_PORTV7
#define LAUNCHER_TAPE_PIN  AD_PORTV8

#define TEST_TAPE_SENSOR      TOP_TAPE_PIN

// Analog to Digital Conversion
#define BLACK_HYSTERESIS 900 // DIGITAL_ONE
#define WHITE_HYSTERESIS 800 // DIGITAL_ZERO

// Hysteresis Cap
#define ROOM_LIGHT_LEVEL 1023

//#define TAPE_DEBUG_VERBOSE
#ifdef TAPE_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

typedef enum {
    BLACK_TO_WHITE, WHITE_TO_BLACK
} tapestate_t;



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
//comment
static uint16_t ReadTapeDetector(uint8_t TapeSensor);

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

static uint16_t ReadTapeDetector(uint8_t TapeSensor){
    uint16_t returnVal;

    if (!AD_IsNewDataReady())
        return FALSE;

    returnVal = AD_ReadADPin(TapeSensor);

    return returnVal;
}
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

ES_Event R2_TopTape() {
    // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevTopTapeState = BLACK_TO_WHITE;

    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;

    // value from ADC
    uint16_t TapeReading = 0;

    tapestate_t NewTopTapeState = PrevTopTapeState; // Temp value

    TapeReading = ReadTapeDetector(TOP_TAPE_PIN); //ADC value

    if(TapeReading == FALSE) //ADC value not ready or ERROR
        return thisEvent;

    // Case: Tape Found
    if ((PrevTopTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = TOP_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTopTapeState = WHITE_TO_BLACK;
    }

    // Case: Tape Lost
    if ((PrevTopTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = TOP_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewTopTapeState = BLACK_TO_WHITE;
    }

    dbprintf("EventParam: %x TapeSensor %x not TapeSensor: %x\n",
            thisEvent.EventParam, TapeSensor, ~TapeSensor);

// PrevTapeState updated with NewTapeState
PrevTopTapeState = NewTopTapeState;

return thisEvent;
}

ES_Event R2_LeftTape(){
        // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevLeftTapeState = BLACK_TO_WHITE;

    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;

    // value from ADC
    uint16_t TapeReading = 0;

    tapestate_t NewLeftTapeState = PrevLeftTapeState; // Temp value

    TapeReading = ReadTapeDetector(LEFT_TAPE_PIN); //ADC value

    if(TapeReading == FALSE) //ADC value not ready or ERROR
        return thisEvent;

    // Case: Tape Found
    if ((PrevLeftTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = LEFT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewLeftTapeState = WHITE_TO_BLACK;
    }

    // Case: Tape Lost
    if ((PrevLeftTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = LEFT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewLeftTapeState = BLACK_TO_WHITE;
    }

    dbprintf("EventParam: %x TapeSensor %x not TapeSensor: %x\n",
            thisEvent.EventParam, TapeSensor, ~TapeSensor);

// PrevTapeState updated with NewTapeState
PrevLeftTapeState = NewLeftTapeState;

return thisEvent;
}


ES_Event R2_RightTape(){
        // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevRightTapeState = BLACK_TO_WHITE;

    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;

    // value from ADC
    uint16_t TapeReading = 0;

    tapestate_t NewRightTapeState = PrevRightTapeState; // Temp value

    TapeReading = ReadTapeDetector(RIGHT_TAPE_PIN); //ADC value

    if(TapeReading == FALSE) //ADC value not ready or ERROR
        return thisEvent;

    // Case: Tape Found
    if ((PrevRightTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = RIGHT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewRightTapeState = WHITE_TO_BLACK;
    }

    // Case: Tape Lost
    if ((PrevRightTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = RIGHT_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewRightTapeState = BLACK_TO_WHITE;
    }

    dbprintf("EventParam: %x TapeSensor %x not TapeSensor: %x\n",
            thisEvent.EventParam, TapeSensor, ~TapeSensor);

// PrevTapeState updated with NewTapeState
PrevRightTapeState = NewRightTapeState;

return thisEvent;
}


ES_Event R2_LauncherTape(){
            // We assume the bot starts in a white area, hence BLACK_TO_WHITE
    static tapestate_t PrevLauncherTapeState = BLACK_TO_WHITE;

    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;

    // value from ADC
    uint16_t TapeReading = 0;

    tapestate_t NewRightTapeState = PrevLauncherTapeState; // Temp value

    TapeReading = ReadTapeDetector(LAUNCHER_TAPE_PIN); //ADC value

    if(TapeReading == FALSE) //ADC value not ready or ERROR
        return thisEvent;

    // Case: Tape Found
    if ((PrevLauncherTapeState == BLACK_TO_WHITE) &&
            (TapeReading > BLACK_HYSTERESIS) &&
            (TapeReading < ROOM_LIGHT_LEVEL)) {
        thisEvent.EventType = TAPE_FOUND;
        thisEvent.EventParam = LAUNCHER_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewRightTapeState = WHITE_TO_BLACK;
    }

    // Case: Tape Lost
    if ((PrevLauncherTapeState == WHITE_TO_BLACK) &&
            (TapeReading < WHITE_HYSTERESIS)) {
        thisEvent.EventType = TAPE_LOST;
        thisEvent.EventParam = LAUNCHER_TAPE_SENSOR;
        Post_R2_BJT2_HSM(thisEvent);
        NewRightTapeState = BLACK_TO_WHITE;
    }

    dbprintf("EventParam: %x TapeSensor %x not TapeSensor: %x\n",
            thisEvent.EventParam, TapeSensor, ~TapeSensor);

// PrevTapeState updated with NewTapeState
PrevLauncherTapeState = NewRightTapeState;

return thisEvent;
}

/*TODO Create R2_Initialize*/
///*************************************************************
// * @Function InitTape()
// * @Param void
// * @Return uint8_t
// * @Author Daniel Ruatta, 11.09.2015
// * @Brief
// * - Calls AD_AddPins on the 4 Tape Sensor pins.
// * - Should return SUCCESS, assuming the IO_Ports were all set to INPUTS.
// * - Will return ERROR if any of the IO_Ports were not set to INPUTS.
// * @Usage
// * - To be called with the rest of our hardware initializations
// */

uint8_t InitTape(void) {
    return AD_AddPins(TOP_TAPE_PIN |
            LEFT_TAPE_PIN |
            RIGHT_TAPE_PIN |
            LAUNCHER_TAPE_PIN | LEFT_TRACK_PIN | RIGHT_TRACK_PIN);
    //| RIGHT_TRACK_PIN | LEFT_TRACK_PIN);
}
