
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TrackWireEvents.h"
#include "AD.h"
#include "R2TapeEvents.h"

// Track Wire
// TODO: Need to refactor events for two track wire sensors -> left/right
#define RIGHT_TRACK_PIN AD_PORTW3
#define LEFT_TRACK_PIN AD_PORTW4

// Analog to Digital Conversion
#define TRACK_WIRE_FOUND_HYSTERESIS 800 // DIGITAL_ONE
#define TRACK_WIRE_LOST_HYSTERESIS 600 // DIGITAL_ZERO

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
ES_Event CheckTrackWire(void) {
    /***************** Declarations ****************/
#define MAX_SENSORS (1<<2)

    static trackwirestate_t PrevTrackWireState = TRACK_WIRE_SEARCHING;
    trackwirestate_t NewTrackWireState = PrevTrackWireState; // Temp value

    uint16_t TrackWireReading;
    //for the sensor loop
    uint8_t sensor;
    uint8_t TrackSensor = 0;
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;

    for (sensor = 1; sensor != MAX_SENSORS; sensor << 1) {
        switch (sensor) {
            case RIGHT_TRACKWIRE_SENSOR:
                dbprintf("Reading Right trackwire\n");
                TrackWireReading = AD_ReadADPin(RIGHT_TRACK_PIN);
                TrackSensor = RIGHT_TRACKWIRE_SENSOR;
                break;
            case LEFT_TRACKWIRE_SENSOR:
                dbprintf("Reading Left trackwire\n");
                TrackWireReading = AD_ReadADPin(LEFT_TRACK_PIN);
                TrackSensor = LEFT_TRACKWIRE_SENSOR;
                break;
            default: break;
        }

        // found the trackwire
        if ((PrevTrackWireState == TRACK_WIRE_SEARCHING) &&
                (TrackWireReading > TRACK_WIRE_FOUND_HYSTERESIS) &&
                (TrackWireReading < HYSTERESIS_CAP)) {
            thisEvent.EventType = TRACK_WIRE_FOUND;
            thisEvent.EventParam |= TrackSensor;
            Post_R2_BJT2_HSM(thisEvent);
            NewTrackWireState = TRACK_WIRE_LOCATED;
        }

        // lost the trackwire
        if ((PrevTrackWireState == TRACK_WIRE_LOCATED) &&
                (TrackWireReading < TRACK_WIRE_LOST_HYSTERESIS)) {
            thisEvent.EventType = TRACK_WIRE_LOST;
            thisEvent.EventParam |= TrackSensor;
            Post_R2_BJT2_HSM(thisEvent);
            NewTrackWireState = TRACK_WIRE_LOST;
        }
        //}

        // Case: Beacon Located
        if ((PrevTrackWireState == TRACK_WIRE_SEARCHING) &&
                (TrackWireReading > TRACK_WIRE_FOUND_HYSTERESIS) &&
                (TrackWireReading < HYSTERESIS_CAP)) {
            thisEvent.EventType = TRACK_WIRE_FOUND;
            Post_R2_BJT2_HSM(thisEvent);
            NewTrackWireState = TRACK_WIRE_LOCATED;
            returnVal = TRUE;
        }

        // Case: Beacon Lost
        if ((PrevTrackWireState == TRACK_WIRE_LOCATED) &&
                (TrackWireReading < TRACK_WIRE_LOST_HYSTERESIS)) {
            thisEvent.EventType = TRACK_WIRE_LOST;
            Post_R2_BJT2_HSM(thisEvent);
            NewTrackWireState = TRACK_WIRE_SEARCHING;
            returnVal = TRUE;
        }
    }

    PrevTrackWireState = NewTrackWireState;

    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return thisEvent;

}

/*************************************************************
 * @Function InitTrackWire()
 * @Param void
 * @Return uint8_t
 * @Author Daniel Ruatta, 11.13.2015
 * @Brief
 * - Calls AD_AddPins on the Beacon Detector pin V04
 * - Should return SUCCESS, assuming the IO_Ports were all set to INPUTS.
 * - Will return ERROR if any of the IO_Ports were not set to INPUTS.
 * @Usage
 * - To be called with the rest of our hardware initializations
 */

uint8_t InitTrackWire() {
    return AD_AddPins(LEFT_TRACK_PIN | RIGHT_TRACK_PIN);
}