/* 
 * File:   driveMotor.h
 * Author: dlavell
 *
 * Created on November 11, 2015, 8:09 PM
 */

#ifndef DRIVEMOTOR_H
#define	DRIVEMOTOR_H

/***************** Public Defines *****************/
#define LEFT_MOTOR  0
#define RIGHT_MOTOR 1
#define FORWARD 30
#define STOP     0

#define R2DriveStraight() R2Motors(FORWARD, FORWARD);
#define R2FullStop() R2Motors(STOP, STOP);

/***************** Function Prototypes *****************
 * 0. InitMotors()
 * 1. motorDirection()
 * 2. setMotorSpeed()
 * 3. rightR2Motor()
 * 4. leftR2Motor()
 * 5. R2Motors()
 * 6. R2DriveStraight()
 * 7. R2FullStop()
 * */

/*************************************************************
 *  Function: InitMotors()
 *
 * Protocol:
 * 1. Call PWM_AddPins and sets direction on IO Pins
 * Notes:
 * - Should return SUCCESS, assuming the IO_Ports were all set to outputs.
 * - Will return ERROR if any of the IO_Ports or PWM ports were not set.
 */

uint8_t initMotors(void);


/******************************************************
 *  Function: motorDirection(uint8_t, uint8_t)
 *
 * Protocol:
 * 1. takes a motor define and a direction
 * 2.
 */

void motorDirection(uint8_t, uint8_t);

/******************************************************
 *  Function: motorSpeed(uint8_t, uint8_t)
 *
 * Protocol:
 * 1. Sets a speed based on battery voltage for each motor
 * 2. returns ERROR or SUCCESS based on SET_PWMDutyCycle
 */

uint8_t motorSpeed(uint16_t, char);

/******************************************************
 *  Function: rightR2Motor(uint8_t)
 *
 * Protocol:
 * 1. Sets a speed based on battery voltage for each motor
 * 2. returns ERROR or SUCCESS based on SET_PWMDutyCycle
 */
uint8_t rightR2Motor(char speed);

/******************************************************

  *  Function: leftR2Motor(uint8_t)
 *
 * Protocol:
 * 1. Sets a speed based on battery voltage for each motor
 * 2. returns ERROR or SUCCESS based on SET_PWMDutyCycle
 */
uint8_t leftR2Motor(char speed);

/******************************************************
 *  Function: leftR2Motor(uint8_t)
 *
 * Protocol:
 * 1. Sets a speed based on battery voltage for each motor
 * 2. returns ERROR or SUCCESS based on SET_PWMDutyCycle
 */
uint8_t R2Motors(char left, char right);


/**
 * @Function R2_BJT2_BatteryVoltage(void)
 * @param None.
 * @return a 10-bit value corresponding to the current voltage of the roach
 * @brief  returns a 10:1 scaled value of the roach battery level
 * @author Max Dunne, 2013.07.12 */
unsigned int R2_BJT2_BatteryVoltage(void);


#endif	/* DRIVEMOTOR_H */

