/*
 * File:   driveMotor.h
 * Author: dlavell
 *
 * Created on November 11, 2015, 8:09 PM
 */

#ifndef DRIVEMOTOR_H
#define	DRIVEMOTOR_H

/***************** Public Defines *****************/
#define DRIVE_FORWARD 30
#define STOP     0

#define R2DriveStraight() R2Motors(DRIVE_FORWARD+2, DRIVE_FORWARD)
#define R2FullStop() R2Motors(STOP, STOP)


/***************** Function Prototypes *****************
 * 0. InitMotors()
 * 1. rightR2Motor()
 * 2. leftR2Motor()
 * 3. R2Motors()
 * 4. R2DriveStraight()
 * 5. R2DriveBack()
 * 6. R2FullStop()
 * 7. R2TankRight()
 * 8. R2TankLeft()
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

uint8_t initLauncherMotor(void);

/******************************************************
 *  Function: rightR2Motor(uint8_t)
 *
 * Protocol:
 * 1. Sets a speed based on battery voltage for each motor
 * 2. returns ERROR or SUCCESS based on SET_PWMDutyCycle
 */
uint8_t R2LauncherMotorSpeed(char speed);


#endif	/* DRIVEMOTOR_H */

