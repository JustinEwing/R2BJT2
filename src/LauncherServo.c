/*
 * File:   driveServo.c
 * Author: dlavell
 *
 */

#include <stdio.h>
#include "xc.h"
#include "serial.h"
#include "pwm.h"
#include "IO_Ports.h"
#include "AD.h"
#include "LauncherServo.h"
#include "RC_Servo.h"

//PWM AND IO Ports for motors
#define LAUNCHER_SERVO   RC_PORTZ08
#define BARREL_OPEN 1000
#define BARREL_CLOSED 1500


//#define MOTOR_DEBUG_VERBOSE
#ifdef Servo_DEBUG_VERBOSE
#include "serial.h"
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

uint8_t initLauncherServo() {
    RC_Init();
    return (RC_AddPins(LAUNCHER_SERVO));
}

uint8_t R2OpenBarrel(){
    return (RC_SetPulseTime(LAUNCHER_SERVO,BARREL_OPEN));
}

uint8_t R2CloseBarrel(){
    return (RC_SetPulseTime(LAUNCHER_SERVO,BARREL_CLOSED));
}
