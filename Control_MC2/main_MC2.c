/*
 * main_MC2.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Nada Amgad
 */
#include "Control_MC2.h"

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/


uint32 g_mismatchCount =0;

uint8 doorPassword[PASSWORD_SIZE]; /*password that will be retreived from the EEPROM*/


int main(void){


	uint8 password_entered[PASSWORD_SIZE];  /*password entered by the user in intermediate cases*/
	uint8 operation;

	uint8 pass_state;  /*match or mismatch*/

	TWI_ConfigType twi_ConfigType = {1,400};


	/* Create configuration structure for UART driver */
	UART_ConfigType uart_ConfigType  = {EIGHT_BITS, EVEN , ONE_BIT ,9600};

	/* Create configuration structure for TIMER driver */
	Timer1_ConfigType timer1_ConfigType ={0 ,8000,PRESCALER_1024,COMPARE_MODE};

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Set the Call back function pointer in the TIMER driver */
	Timer1_setCallBack(MC2_incrementCounter);

	DcMotor_Init();  /*Initialize the DC MOTOR driver*/
	Buzzer_init();
	TWI_init(&twi_ConfigType);
	Timer1_init(&timer1_ConfigType); /*Initialize the Timer driver*/
	UART_init(&uart_ConfigType); /*Initialize the UART driver*/

	MC2_verifyPasswords();
	MC2_getSavedPassword(doorPassword);  /*get the saved password for further easier checking*/

	uint8 uart_byte;
	while(1){

		//while((UART_recieveByte() != OPEN_DOOR) || (UART_recieveByte() != CHANGE_PASS));
		uart_byte=UART_recieveByte();
		if(uart_byte == OPEN_DOOR){
			do{

				while(UART_recieveByte() != RECEIVE_PASS);
				UART_sendByte(SEND_PASS);
				MC2_receivePassword(password_entered);

				pass_state = MC2_checkPasswords(password_entered, doorPassword);

				if(pass_state == TRUE){
					UART_sendByte(DOOR_UNLOCKING);
					MC2_manageDoorOperation();
					g_mismatchCount = 0; /*reset the mismatch count*/
					break;
				}
				else if(pass_state == FALSE){
					g_mismatchCount++;
					UART_sendByte(MISMATCH);
				}


			}while(g_mismatchCount < 3);

			if(g_mismatchCount == 3){
				MC2_warningState();
				g_mismatchCount = 0;
			}
		}
		else if(uart_byte == CHANGE_PASS){
			do{

				while(UART_recieveByte() != RECEIVE_PASS);
				UART_sendByte(SEND_PASS);
				MC2_receivePassword(password_entered);


				pass_state = MC2_checkPasswords(password_entered, doorPassword);

				if(pass_state == TRUE){

					UART_sendByte(CHANGE_PASSWORD);
					MC2_verifyPasswords();
					MC2_getSavedPassword(doorPassword);  /*get the saved password for further easier checking*/
					g_mismatchCount = 0; /*reset the mismatch count*/
					break;
				}
				else if(pass_state == FALSE){
					g_mismatchCount++;
					UART_sendByte(MISMATCH);

				}


			}while(g_mismatchCount < 3);

			if(g_mismatchCount == 3){
				MC2_warningState();
				g_mismatchCount = 0;
			}
		}
	}

}

