 /******************************************************************************
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for the dc motor driver
 *
 * Author: Nada Amgad
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_PORT_ID                   PORTD_ID
#define DC_MOTOR_PIN_ID_1                  PIN6_ID
#define DC_MOTOR_PIN_ID_2                  PIN7_ID

typedef enum {
	STOP, CLOCKWISE, ANTICLOCKWISE
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description:
 * The Function responsible for setup the direction for the
 * two motor pins through the GPIO driver. Stop at the DC-Motor at the beginning
 * through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.Send the required duty cycle to the PWM driver based on the
 * required speed value
 * Input:
 *  1.state: The required DC Motor state, it should be CW or A-CW or stop
 *  2.speed: decimal value for the required motor speed, it should be from 0 → 100.
 */

void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
