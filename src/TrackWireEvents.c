
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TrackWireEvents.h"
#include "AD.h"
#include "R2Events.h"

// Analog to Digital Conversion
#define TRACK_WIRE_FOUND_HYSTERESIS 400 // DIGITAL_ONE
#define TRACK_WIRE_LOST_HYSTERESIS 300 // DIGITAL_ZERO

// Hysteresis Cap
#define HYSTERESIS_CAP 1023

//#define TRACKWIRE_DEBUG_VERBOSE
#ifdef TRACKWIRE_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

typedef enum {
    TRACK_WIRE_SEARCHING, TRACK_WIRE_LOCATED
} trackwirestate_t;

/**************************************************************************
 * @Function CheckTrackWire
 * @Param void
 * @Return uint8_t so that the ES_Framework can call it
 * @Author Daniel Ruatta, 11.13.2015
 * @Brief
 * 1. Gets the Track Wire's analog value
 * 2. Checks if this reading has passed either of its
 * Hysteresis Thresholds
 * 3. If so, post an event to the TrackWireHSM
 * 4. Return TRUE if an event has been posted, FALSE otherwise
 */
ES_Event ReadTrackWire(void) {
    /***************** Declarations ****************/
    static trackwirestate_t PrevLeftState = TRACK_WIRE_SEARCHING;
    static trackwirestate_t PrevRightState = TRACK_WIRE_SEARCHING;
    static trackwirestate_t NewLeftState = TRACK_WIRE_SEARCHING;
    static trackwirestate_t NewRightState = TRACK_WIRE_SEARCHING;
    uint16_t LeftReading;
    uint16_t RightReading;
    uint8_t param = 0; // will have left track wire as the second bit and right
    // track wire as the least significant bit
    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;

    LeftReading = AD_ReadADPin(LEFT_TRACK_PIN);
    dbprintf(": %d\n", LeftReading);
    RightReading = AD_ReadADPin(RIGHT_TRACK_PIN);
    dbprintf("Reading Right trackwire: %d\n", RightReading);

    // Test Left
    if ((LeftReading > TRACK_WIRE_FOUND_HYSTERESIS) &&
            (PrevLeftState == TRACK_WIRE_SEARCHING)) { // Left Found
        NewLeftState = TRACK_WIRE_LOCATED;
        param |= LEFT_TRACKWIRE_SENSOR;
    } else if ((LeftReading < TRACK_WIRE_LOST_HYSTERESIS) &&
            (PrevLeftState == TRACK_WIRE_LOCATED)) { // Left Lost
        NewLeftState = TRACK_WIRE_SEARCHING;
    }

    // Test Right
    if ((RightReading > TRACK_WIRE_FOUND_HYSTERESIS) &&
            (PrevRightState == TRACK_WIRE_SEARCHING)) { // Right Found
        NewRightState = TRACK_WIRE_LOCATED;
        param |= RIGHT_TRACKWIRE_SENSOR;
    } else if ((RightReading < TRACK_WIRE_LOST_HYSTERESIS) &&
            (PrevRightState == TRACK_WIRE_LOCATED)) { // Right Lost
        NewRightState = TRACK_WIRE_SEARCHING;
    }

    // Determine if there was an event
    if (((param & LEFT_TRACKWIRE_SENSOR) && (PrevLeftState == TRACK_WIRE_SEARCHING)) ||
            ((param & RIGHT_TRACKWIRE_SENSOR) && (PrevRightState == TRACK_WIRE_SEARCHING))) {
        printf("PrevLState: %d\n", PrevLeftState);
        printf("NextLState: %d\n", NewLeftState);
        printf("Param: %d\n", param);
        thisEvent.EventType = TRACK_WIRE_FOUND;
        thisEvent.EventParam = param;

    }
    PrevLeftState = NewLeftState;
    PrevRightState = NewRightState;

    return thisEvent;

}
