 /******************************************************************************
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the dc motor driver
 *
 * Author: Nada Amgad
 *
 *******************************************************************************/
#include "dc_motor.h"
#include "pwm.h"
#include "gpio.h"

/*
 * Description:
 * The Function responsible for setup the direction for the
 * two motor pins through the GPIO driver. Stop at the DC-Motor at the beginning
 * through the GPIO driver.
 */
void DcMotor_Init(void){
/*configure motor pins as output*/
GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_PIN_ID_1, PIN_OUTPUT);
GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_PIN_ID_2, PIN_OUTPUT);

/*Stop the motor at the beginning*/
GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_1 , LOGIC_LOW);
GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_2 , LOGIC_LOW);
}


/*
 * Description:
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.Send the required duty cycle to the PWM driver based on the
 * required speed value
 * Input:
 *  1.state: The required DC Motor state, it should be CW or A-CW or stop
 *  2.speed: decimal value for the required motor speed, it should be from 0 → 100.
 */

void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	PWM_Timer0_Start((uint8)((uint32)(speed * TIMER_MAX_VALUE)/100));

	if(state == STOP){
		/*stop the motor*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_1 , LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_2 , LOGIC_LOW);
	}
	else if (state == CLOCKWISE){
		/*rotate clockwise*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_1 , LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_2 , LOGIC_LOW);
	}
	else if(state == ANTICLOCKWISE){
		/*rotate anti-clockwise*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_1 , LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_ID_2 , LOGIC_HIGH);
	}

}
