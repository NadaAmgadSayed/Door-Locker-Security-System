/*
 * main_MC1.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Nada Amgad
 */
#include "HMI_MC1.h"

uint8 g_mismatchCount;
uint8 Pass[PASSWORD_SIZE], confimedPass[PASSWORD_SIZE];

int main(void){

	uint8 key = 0;

	/* Create configuration structure for UART driver */
	UART_ConfigType uart_ConfigType  = {EIGHT_BITS, EVEN , ONE_BIT ,9600};

	/* Create configuration structure for TIMER driver */
	Timer1_ConfigType timer1_ConfigType ={0 ,8000,PRESCALER_1024,COMPARE_MODE};

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Set the Call back function pointer in the TIMER driver */
	Timer1_setCallBack(MC1_incrementCounter);

	LCD_init();
	Timer1_init(&timer1_ConfigType); /*Initialize the Timer driver*/
	UART_init(&uart_ConfigType); /*Initialize the UART driver*/


	MC1_createPassword();

	while(1){

		MC1_displayMainmenu(); /*display the main menu +: open door  -: change password */

		key = KEYPAD_getPressedKey();
		_delay_ms(500);

		if(key == '+'){

			UART_sendByte(OPEN_DOOR);  /*send operation to MC2*/

			do{
				MC1_enterPassword(PASSWORD); /*get the password from the user and save it in Pass[PASSWORD_SIZE] defined globally */

				/****SENDING THE 1st PASS TO MC2****/

				UART_sendByte(RECEIVE_PASS); /*check that the MC2 is ready to receive password*/

				while(UART_recieveByte() != SEND_PASS);  /*wait till MC2 is ready to Receive pass if not stay in the loop*/

				MC1_sendPassword(Pass); /*Send the 1st password to MC2*/


				/*wait for the uart in MC2 to transmit the correct action*/
				uint8 receivedByte = UART_recieveByte();
				_delay_ms(500);

				/*if passwords match in MC2 open the door operation*/
				if((receivedByte == DOOR_UNLOCKING)){
					g_mismatchCount = 0; /*reset the mismatch count*/
					MC1_displayDoorOperation();
					break; /*to exit the do while loop*/
				}
				else if((receivedByte == MISMATCH)){
					g_mismatchCount++;  /*increment mismatch counter by 1*/
					LCD_clearScreen();
					LCD_displayString("WRONG PASS !!");
					_delay_ms(500);

				}
			}while(g_mismatchCount < 3);

			if(g_mismatchCount == 3){
				MC1_warningMessage();  /*to display warning message for 1 min*/
				g_mismatchCount = 0; /*reset the mismatch count*/
			}

		}
		else if(key == '-'){

			UART_sendByte(CHANGE_PASS);

			do{

				MC1_enterPassword(PASSWORD); /*get the password from the user and save it in Pass[PASSWORD_SIZE] defined globally */

				/****SENDING THE 1st PASS TO MC2****/

				UART_sendByte(RECEIVE_PASS); /*check that the MC2 is ready to receive password*/

				while(UART_recieveByte() != SEND_PASS);  /*wait till MC2 is ready to Receive pass if not stay in the loop*/

				MC1_sendPassword(Pass); /*Send the 1st password to MC2*/


				uint8 receivedByte = UART_recieveByte();

				if((receivedByte == CHANGE_PASSWORD)){
					g_mismatchCount = 0; /*reset the mismatch count*/
					MC1_createPassword();
					break; /*to exit the do while loop*/
				}
				else if((receivedByte == MISMATCH)){
					g_mismatchCount++;  /*increment mismatch counter by 1*/
					LCD_clearScreen();
					LCD_displayString("WRONG PASS !!");
					_delay_ms(500);
				}

			}while(g_mismatchCount < 3);

			if(g_mismatchCount == 3){
				MC1_warningMessage();  /*to display warning message for 1 min*/
				g_mismatchCount = 0; /*reset the mismatch count*/
			}
		}

	}

}
