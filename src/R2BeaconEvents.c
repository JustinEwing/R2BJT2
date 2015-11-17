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
#define BEACON_INIT PORTY03_TRIS //Need to change
#define INPUT 1

//#define BEACON_DEBUG_VERBOSE
#ifdef BEACON_DEBUG_VERBOSE
#include "serial.h"
#include <stdio.h>
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif


uint8_t InitBeacon(void) {
    return IO_PortsSetPortInputs(BEACON_INIT, INPUT);
}