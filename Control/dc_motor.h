 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for the DC_MOTOR driver
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID            PORTB_ID

#define DC_MOTOR_IN1_PIN_ID         PIN0_ID
#define DC_MOTOR_IN2_PIN_ID         PIN1_ID

#define DC_MOTOR_EN1_PORT_ID        PORTB_ID
#define DC_MOTOR_EN1_PIN_ID         PIN2_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	STOP,CLOCK_WISE,ANTI_CLOCK_WISE
}dcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void dcMotor_Init(void);

void dcMotor_Rotate(dcMotor_State state);

#endif /* DC_MOTOR_H_ */
