/*/*
 * File:   driveMotors.c
 * Author: jpewing
 *
 * Created on November 9, 2015
 */

#include <stdio.h>
#include "xc.h"
#include "serial.h"
#include "pwm.h"
#include "IO_Ports.h"
#include "AD.h"
#include "driveMotor.h"

/************************** #DEFINES **********************
 * - 2 Driver Motors
 *  - Using PortY12 for Left Motor Enable
 *  - Using PortY11 for Left Motor Direction
 *  - Using PortY10 for Right Motor Enable
 *  - Using PortY09 for Right Motor Direction
 *
 *
 */

//PWM AND IO Ports for motors
#define LEFT_MOTOR_ENABLE   PWM_PORTY12
#define RIGHT_MOTOR_ENABLE  PWM_PORTY10

#define LEFT_MOTOR_DIR      PORTY11_LAT
#define RIGHT_MOTOR_DIR     PORTY09_LAT

#define LEFT_MOTOR_PIN_DIR  PORTY11_TRIS
#define RIGHT_MOTOR_PIN_DIR PORTY09_TRIS



//DVR8814 direction pin inputs
#define FORWARD 1
#define BACK    0

// I/O
#define INPUT  1
#define OUTPUT 0

//PWM settings
#define PWM_FREQ PWM_DEFAULT_FREQUENCY

/*Uncomment to enable test harness, right now its an input driven to scale the
 * duty cycle from 0-9 (with keys 0-9) and full power (key p)
 * motor direction can be toggled with keys f and b for forward and back
 * motor has to be changed as a #define at the moment */

//#define MOTOR_TEST
//uint16_t TEST_MOTOR = RIGHT_MOTOR;

//#define MOTOR_DEBUG_VERBOSE
#ifdef MOTOR_DEBUG_VERBOSE
#include "serial.h"
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

/********************* End of #defines **********************/

/*********************************************************
 ******************* Function Definitions ****************
 * ******************************************************/

uint8_t initMotors() {
    PWM_Init();
    PWM_SetFrequency(PWM_FREQ);

    //set I/O direction for h-bridge directions pins
    RIGHT_MOTOR_PIN_DIR = OUTPUT;
    LEFT_MOTOR_PIN_DIR = OUTPUT;

    return (PWM_AddPins(LEFT_MOTOR_ENABLE | RIGHT_MOTOR_ENABLE));
}

/*************************************************************
 *  Function: motorDirection(uint8_t motor, uint8_t direction)

 * @param motor, use #defined LEFT_MOTOR or RIGHT_MOTOR
 * @param direction, use #defined FORWARD or BACK
 * @return void
 * @brief Function sets the director for the motor direction pin */

void motorDirection(uint8_t motor, uint8_t direction) {
    if (motor == RIGHT_MOTOR) {
        RIGHT_MOTOR_DIR = direction;
    } else {
        LEFT_MOTOR_DIR = direction;
    }
}

/*************************************************************
 *  Function: motorSpeed(uint8_t motor, uint8_t speed)

 * @param motor, use #defined LEFT_MOTOR or RIGHT_MOTOR
 * @param speed, where speed is 0-10,
 * 0 being stopped and 10 full speed
 * @return ERROR if PWM_setDUTYCycle failed to set, or SUCCESS
 * @brief Function sets the speed of the motor direction pin */

/*TODO --Function to scale with Battery level
         and cap at 60% duty cycle after function
 */

uint8_t motorSpeed(uint16_t motor, char speed) {
    dbprintf("Entered %s with speed %d\n", __FUNCTION__, speed);
    unsigned int newSpeed;
    uint16_t SELECTED_MOTOR;
    uint16_t SELECTED_DIR;

    if (speed > 10 || speed < -10)
        return ERROR;

    //Cap the Duty Cycle at 60% for now
    if (speed > 6)
        speed = 6;
    if (speed < -6)
        speed = -6;

    switch (motor) {
        case RIGHT_MOTOR:
            dbprintf("Selected Right Motor\n");
            SELECTED_MOTOR = RIGHT_MOTOR_ENABLE;

            if (speed < 0) {
                dbprintf("%s: Set to backwards\n", __FUNCTION__);
                SELECTED_DIR = BACK;
                newSpeed = speed * (-1); //Set as positive value
            } else {
                dbprintf("%s: Set to Forward\n", __FUNCTION__);
                SELECTED_DIR = FORWARD;
                newSpeed = speed;
            }
            break;

        case LEFT_MOTOR:
            dbprintf("Selected Left Motor\n");
            SELECTED_MOTOR = LEFT_MOTOR_ENABLE;

            if (speed < 0) {
                dbprintf("%s Set to backwards\n", __FUNCTION__);
                SELECTED_DIR = BACK;
                newSpeed = speed * (-1); //Set as positive value
            } else {
                dbprintf("%s: Set to Forward\n", __FUNCTION__);
                SELECTED_DIR = FORWARD;
                newSpeed = speed;
            }
            break;
    }

    //Set the direction of drive
    motorDirection(SELECTED_MOTOR, SELECTED_DIR);

    //set the speed
    if ((PWM_SetDutyCycle(SELECTED_MOTOR, newSpeed * 100)) != SUCCESS) {
        return ERROR;
    }


    return SUCCESS;
}

/*************************************************************
 *  Function: rightR2Motor(uint8_t speed)
 *
 * @param speed, where speed is -100 - 100,
 * 0 being stopped and -10 full reverse and 10 being full forward
 * @return ERROR if PWM_setDUTYCycle failed to set, or SUCCESS
 * @brief Function sets the speed of the motor direction pin */

/*TODO --Function to scale with Battery level
         and cap at 60% duty cycle after function
 */

uint8_t rightR2Motor(char speed){
    dbprintf("Entered %s with speed %d\n", __FUNCTION__, speed);
    unsigned int newSpeed;

    if (speed > 100 || speed < -100)
        return ERROR;

    //Cap the Duty Cycle at 60% for now
    if (speed > 60)
        speed = 60;
    if (speed < -60)
        speed = -60;

     if (speed < 0) {
        dbprintf("%s: Set to backwards\n", __FUNCTION__);
        RIGHT_MOTOR_DIR = BACK;
        newSpeed = speed * (-1); //Set as positive value
    } else {
        dbprintf("%s: Set to Forward\n", __FUNCTION__);
        RIGHT_MOTOR_DIR = FORWARD;
        newSpeed = speed;
    }
    //set the speed
    if ((PWM_SetDutyCycle(RIGHT_MOTOR_ENABLE, newSpeed * 10)) != SUCCESS) {
        return ERROR;
    }
    return SUCCESS;
}

/*************************************************************
 *  Function: leftR2Motor(uint8_t speed)

 * @param speed, where speed is 0-10,
 * 0 being stopped and -10 full reverse and 10 being full forward
 * @return ERROR if PWM_setDUTYCycle failed to set, or SUCCESS
 * @brief Function sets the speed of the motor direction pin */

/*TODO --Function to scale with Battery level
         and cap at 60% duty cycle after function
 */

uint8_t leftR2Motor(char speed){
    dbprintf("Entered %s with speed %d\n", __FUNCTION__, speed);
    unsigned int newSpeed;

    if (speed > 100 || speed < -100)
        return ERROR;

    //Cap the Duty Cycle at 60% for now
    if (speed > 60)
        speed = 60;
    if (speed < -60)
        speed = -60;

     if (speed < 0) {
        dbprintf("%s: Set to backwards\n", __FUNCTION__);
        LEFT_MOTOR_DIR = BACK;
        newSpeed = speed * (-1); //Set as positive value
    } else {
        dbprintf("%s: Set to Forward\n", __FUNCTION__);
        LEFT_MOTOR_DIR = FORWARD;
        newSpeed = speed;
    }
    //set the speed
    if ((PWM_SetDutyCycle(LEFT_MOTOR_ENABLE, newSpeed * 10)) != SUCCESS) {
        return ERROR;
    }
    return SUCCESS;
}

uint8_t R2Motors(char left, char right){
    if(leftR2Motor(left) == ERROR)
        return ERROR;
    if(rightR2Motor(right) == ERROR)
        return ERROR;

    return SUCCESS;

}

/**
 * @Function R2_BJT2_BatteryVoltage(void)
 * @param None.
 * @return a 10-bit value corresponding to the current voltage of the roach
 * @brief  returns a 10:1 scaled value of the roach battery level
 * @author Max Dunne, 2013.07.12 */
unsigned int R2_BJT2_BatteryVoltage(void) {
    return AD_ReadADPin(BAT_VOLTAGE);
}
/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

#ifdef MOTOR_TEST

#include <stdio.h>
#include "xc.h"
#include "serial.h"
#include "pwm.h"
#include "IO_Ports.h"

void char_input(unsigned int input);

//  Delays for testing
#define DELAY(x)    for (wait = 0; wait <= x; wait++) {asm("nop");}
#define SLOW 10000
#define A_BIT  5000

unsigned int wait;

void print_Duty() {
    dbprintf("Left Motor Duty: %d\nRight Motor Duty: %d\n\n",
            PWM_GetDutyCycle(LEFT_MOTOR_ENABLE) / 10,
            PWM_GetDutyCycle(RIGHT_MOTOR_ENABLE) / 10);


}

int main(void) {
    BOARD_Init();
    SERIAL_Init();

    //Init the motors, duh
    initMotors();

    char input;

    dbprintf("Welcome to the Motor Test Harness\n");
    dbprintf("Speed: 0-10, f: forward, s: backward\n");
    dbprintf("r: right motor, l: left motor, b: battery\n");

    while (1) {
        input = GetChar();

        char_input(input);

        DELAY(A_BIT)
    }
    return 0;
}

void char_input(unsigned int input) {
    switch (input) {
        case '0':
            motorSpeed(TEST_MOTOR, -5);
            print_Duty();
            break;
        case '1':
            motorSpeed(TEST_MOTOR, -4);
            print_Duty();
            break;
        case '2':
            motorSpeed(TEST_MOTOR, -3);
            print_Duty();
            break;
        case '3':
            motorSpeed(TEST_MOTOR, -2);
            print_Duty();
            break;
        case '4':
            motorSpeed(TEST_MOTOR, -1);
            print_Duty();
            break;
        case '5':
            motorSpeed(TEST_MOTOR, 0);
            print_Duty();
            break;
        case '6':
            motorSpeed(TEST_MOTOR, 1);
            print_Duty();
            break;
        case '7':
            motorSpeed(TEST_MOTOR, 2);
            print_Duty();
            break;
        case '8':
            motorSpeed(TEST_MOTOR, 3);
            print_Duty();
            break;
        case '9':
            motorSpeed(TEST_MOTOR, 4);
            print_Duty();
            break;
        case '+':
            motorSpeed(TEST_MOTOR, 8);
            print_Duty();
            break;
        case '-':
            motorSpeed(TEST_MOTOR, -6);
            print_Duty();
            break;
        case 'w':
            motorDirection(TEST_MOTOR, FORWARD);
            printf("Forward\n");
            break;
        case 's':
            motorDirection(TEST_MOTOR, BACK);
            printf("Back\n");
            break;
        case'b':
            dbprintf("Battery: %d\n", R2_BJT2_BatteryVoltage());
            break;
        case'r':
            TEST_MOTOR = RIGHT_MOTOR;
            dbprintf("Right Motor Selected\n");
            break;
        case'l':
            TEST_MOTOR = LEFT_MOTOR;
            dbprintf("Left Motor Selected\n");
            break;
            //printf("afterdefault");
            break;
    }
}

#endif