/*
 * Control_MC2.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Nada Amgad
 */


#include "Control_MC2.h"

uint8 receivedPassword_1[PASSWORD_SIZE];  /*array in which password will be received*/
uint8 receivedPassword_2[PASSWORD_SIZE];  /*array in which password will be received*/
uint32 g_ticks = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description: Function to receive the password the user inputs using uart from MC1.
 * Inputs: uint8* password
 * Returns: none
 */
void MC2_receivePassword(uint8* receivedPassword){

	uint8 i;
	for (i=0 ; i< PASSWORD_SIZE ;i++){
		receivedPassword[i]= UART_recieveByte(); /*receive the character i from MC1 using UART*/
		_delay_ms(50);
	}

}


/*
 * Description:Function to compare passwords and check correctness
 * Inputs: uint8* password, uint8* confirmed_password that are saved in arrays
 * Returns: True or False "0 or 1" of type uint8
 */
uint8 MC2_checkPasswords(uint8* Password ,uint8* confirmPassword){

	uint8 i;
	for (i=0 ; i< PASSWORD_SIZE ;i++){

		if(Password[i] != confirmPassword[i]){
			return FALSE;  /*if 1 character mismatched the other then return false*/
		}

	}

	return TRUE; /*two passwords match*/
}


/*
 * Description:Function to verify passwords and communicate with uart MC1 then saves the correct pass in the EEPROM
 * Inputs: none
 * Returns: none
 */
void MC2_verifyPasswords(void){

	uint8 mismatch = TRUE;
	uint8 state= 0;

	do{
		/****RECEIVING THE 1st PASS FROM MC1****/
		while(UART_recieveByte() != RECEIVE_PASS); /*wait till uart MC1 is ready and asks you to receive password 1*/
		UART_sendByte(SEND_PASS);  /*inform uart in MC1 to start sending*/
		MC2_receivePassword(receivedPassword_1);

		/****RECEIVING THE 2nd PASS FROM MC1****/
		while(UART_recieveByte() != RECEIVE_PASS); /*wait till uart MC1 is ready and asks you to receive confirmed password*/
		UART_sendByte(SEND_PASS);  /*inform uart in MC1 to start sending*/
		MC2_receivePassword(receivedPassword_2);

		/*check if passwords match or mismatch and save them in state variable*/
		state = MC2_checkPasswords(receivedPassword_1, receivedPassword_2);

		_delay_ms(100);
		if(state == 1){
			UART_sendByte(MATCH);  /*send state to UART in MC1*/
			mismatch = FALSE;  /*invert the flag to exit the loop*/
		}
		else if(state == 0){
			UART_sendByte(MISMATCH);  /*send state to UART in MC1*/
		}
	}while(mismatch);


	if(state == TRUE){ /*if passwords match save them in the EEPROM*/
		MC2_savePassword(receivedPassword_1);
	}


}


/*
 * Description:This is the call back function called by the Timer driver.
 * Inputs: none
 * Returns: none
 */
void MC2_incrementCounter(void){
	g_ticks++;
}


/*
 * Description: Function to store the password in the EEPROM in a specified save address
 * Inputs: uint8* password  "Password that the user inputs and confirmed in array"
 * Returns: none
 */
void MC2_savePassword(uint8* password){

	uint8 i ;
	for(i = 0; i < PASSWORD_SIZE ; i++){

		EEPROM_writeByte((SAVE_ADDRESS+i), password[i]); /*writing the character i of the password in the EEPROM*/
		_delay_ms(10);

	}

}

/*
 * Description: Function to get the saved password in the EEPROM in a specified save address
 * Inputs: uint8* password  "Array in witch the password will be returned from the EEPROM"
 * Returns: none
 */
void MC2_getSavedPassword(uint8* password){

	uint8 i ;
	for(i = 0; i < PASSWORD_SIZE ; i++){

		EEPROM_readByte((SAVE_ADDRESS+i), &password[i]);/*getting the character i of the password from the EEPROM*/
		_delay_ms(10);

	}

}


/*
 * Description: Function to Manage Door operations "LOCKING, UNLOCKING, LOCKED".
 * Inputs: none
 * Returns: none
 */
void MC2_manageDoorOperation(void){

	/**Door Opens**/
	g_ticks = 0;  /*reset the ticks to count 15 sec from the start*/

	DcMotor_Rotate(CLOCKWISE, 100);
	while(g_ticks < DOOR_OPEN_TIME){
	}


	/**Door Held**/
	g_ticks = 0; /*reset the ticks to count 3 sec from the start*/
	DcMotor_Rotate(STOP, 0);
	while(g_ticks < DOOR_HOLD_TIME){
	}


	/**Door Closes**/
	g_ticks = 0;  /*reset the ticks to count 15 sec from the start*/
	DcMotor_Rotate(ANTICLOCKWISE, 100);
	while(g_ticks < DOOR_CLOSE_TIME){
	}

	DcMotor_Rotate(STOP, 0);
	g_ticks = 0;  /*reset the ticks*/

}


/*
 * Description: Function to activate buzzer when the user enter wrong password certain number of times
 * Inputs: none
 * Returns: none
 */
void MC2_warningState(void){
	g_ticks=0; /*reset the ticks to count 1 min from the start*/

	/*Display an Error message for 1 min*/
	while(g_ticks < 60){
		Buzzer_on();
	}

		Buzzer_off();
	g_ticks=0; /*reset the ticks*/

}
