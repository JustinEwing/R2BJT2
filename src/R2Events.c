#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "R2Events.h"
#include "R2_BJT2_HSM.h"
#include "IO_Ports.h"
#include "AD.h"


/******************************* Private #defines ****************************/



#define BEACON_PORT PORTY //used for init
#define BEACON_PIN PIN5
#define BEACON_READ PORTY05_BIT

typedef enum {
    WAS_FOUND, WAS_LOST
} beaconstate_t;

static beaconstate_t PrevBeaconState = WAS_LOST;


//define EVENTS_DEBUG_VERBOSE
#ifdef EVENTS_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif


uint8_t CheckTopTape(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    /***************** Declarations ****************/
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;

    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    thisEvent = R2_TopTape();

    //If an event has happened.
    if(thisEvent.EventType != ES_NO_EVENT) {
        Post_R2_BJT2_HSM(thisEvent);
        returnVal = TRUE;
    }

    dbprintf("Exit %s\n", __FUNCTION__);
    return returnVal;
}

uint8_t CheckLeftTape(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    /***************** Declarations ****************/
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;

    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    thisEvent = R2_LeftTape();

    //If an event has happened.
    if(thisEvent.EventType != ES_NO_EVENT) {
        Post_R2_BJT2_HSM(thisEvent);
        returnVal = TRUE;
    }

    dbprintf("Exit %s\n", __FUNCTION__);
    return returnVal;
}

uint8_t CheckRightTape(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    /***************** Declarations ****************/
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;

    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    thisEvent = R2_RightTape();

    //If an event has happened.
    if(thisEvent.EventType != ES_NO_EVENT) {
        Post_R2_BJT2_HSM(thisEvent);
        returnVal = TRUE;
    }

    dbprintf("Exit %s\n", __FUNCTION__);
    return returnVal;
}

uint8_t CheckLauncherTape(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    /***************** Declarations ****************/
    // thisEvent, which will be posted to the HSM on a TAPE_FOUND or
    //TAPE_LOST event
    ES_Event thisEvent;

    // returnVal, which will be used by the ES_Framework to see if this event
    // posted an event. We assume no event initially happens, hence FALSE
    uint8_t returnVal = FALSE;

    thisEvent = R2_LauncherTape();

    //If an event has happened.
    if(thisEvent.EventType != ES_NO_EVENT) {
        Post_R2_BJT2_HSM(thisEvent);
        returnVal = TRUE;
    }

    dbprintf("Exit %s\n", __FUNCTION__);
    return returnVal;
}

uint8_t CheckTrackWire(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    uint8_t returnVal = FALSE;
    ES_Event thisEvent = ReadTrackWire();
    //If an event has happened.
    if (thisEvent.EventType != ES_NO_EVENT) {
        returnVal = TRUE;
        Post_R2_BJT2_HSM(thisEvent);
    }
    return returnVal;
}

uint8_t CheckBeacon(void) {
    dbprintf("Entered %s\n", __FUNCTION__);
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint8_t beacon = BEACON_READ;
    //printf("%d\n",beacon); // quick and dirty
    
    // NOTE: beacon detector circuit is active low
    if (!beacon && (PrevBeaconState == WAS_LOST)) {
        dbprintf("Found da beacon\n");
        PrevBeaconState = WAS_FOUND;
        thisEvent.EventType = BEACON_FOUND;
        thisEvent.EventParam = 1;
        Post_R2_BJT2_HSM(thisEvent);
        returnVal = TRUE;
    } else if (beacon && (PrevBeaconState == WAS_FOUND)) {
        dbprintf("Lost da beacon\n");
        PrevBeaconState = WAS_LOST;
        thisEvent.EventType = BEACON_LOST;
        thisEvent.EventParam = 0;
        Post_R2_BJT2_HSM(thisEvent);
    }
    return returnVal;
}

uint8_t InitBeacon(void) {
    return IO_PortsSetPortInputs(BEACON_PORT, BEACON_PIN);
}

uint8_t InitTrackWire(void) {
    return 0;// AD_AddPins(RIGHT_TRACK_PIN | LEFT_TRACK_PIN);
}

