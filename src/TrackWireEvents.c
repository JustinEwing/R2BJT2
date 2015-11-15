#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TrackWireEvents.h"
#include "R2_BJT2_HSM.h"
#include "IO_Ports.h"
#include "AD.h"

// Track Wire
#define TRACK_WIRE_INIT PORTV04_TRIS
#define TRACK_WIRE_READ PORTV04_BIT

// Analog to Digital Conversion
#define TRACK_WIRE_FOUND_HYSTERESIS 800 // DIGITAL_ONE
#define TRACK_WIRE_LOST_HYSTERESIS 600 // DIGITAL_ZERO

// Hysteresis Cap
#define HYSTERESIS_CAP 1023

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
uint8_t CheckTrackWire(void) {
    /***************** Declarations ****************/
    static trackwirestate_t PrevTrackWireState = TRACK_WIRE_SEARCHING;

    trackwirestate_t NewTrackWireState = PrevTrackWireState; // Temp value

    uint8_t TrackWireReading;

    ES_Event thisEvent;
    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    TrackWireReading = AD_ReadADPin(TRACK_WIRE_READ);

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

    PrevTrackWireState = NewTrackWireState;

    // Return TRUE if an event has been detected and posted
    // FALSE otherwise
    return returnVal;

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
    return AD_AddPins(TRACK_WIRE_INIT);
}