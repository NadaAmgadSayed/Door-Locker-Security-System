/*
 * HMI_MC1.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Nada Amgad
 */

#include "HMI_MC1.h"




uint32 g_ticks = 0;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description: Function to take the password input from the user.
 * Inputs: uint8 status  "PASSWORD or CONFIRM_PASSWORD" to display the correct commands
 * Returns: none
 */
void MC1_enterPassword(uint8 status){

	uint8 i=0;

	LCD_clearScreen();
	LCD_moveCursor(0, 0);
	LCD_displayString("Plz enter pass:");
	if(status == PASSWORD){

		uint8 Key_Pressed;

		/*Move a cursor of LCD to second Row to Print Password Entered by user */
		LCD_moveCursor(1, 0);

		/* Take Pressed key from user until */
		for(uint8 i = 0 ; i < PASSWORD_SIZE ; i++)
		{
			/* Get Key_Pressed by user */
			Key_Pressed = KEYPAD_getPressedKey();

			/* Store Password */
			Pass[i] = Key_Pressed;

			/* Display on LCD Encrypted PAssword (*) */
			LCD_displayCharacter('*');

//			uint8 num = Pass[i];
//			LCD_intgerToString(num);

			_delay_ms(200);
		}

		/* Don't Exit From this Fucntion Until user entered Enter Key */
		while(KEYPAD_getPressedKey() != ENTER_KEY){}
	}
	else if(status == CONFIRM_PASSWORD){

		uint8 Key_Pressed;
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Plz reenter pass:");
		/*Move a cursor of LCD to second Row to Print Password Entered by user */
		LCD_moveCursor(1, 0);

		for(uint8 i = 0 ; i < PASSWORD_SIZE ; i++)
		{
			/* Get Key_Pressed by user */
			Key_Pressed = KEYPAD_getPressedKey();
			//_delay_ms(500);

			/* Store Password */
			confimedPass[i] = Key_Pressed;

			/* Display on LCD Encrypted PAssword (*) */
			LCD_displayCharacter('*');
//			uint8 num = confimedPass[i];
//			LCD_intgerToString(num);

			_delay_ms(200);
		}

		/* Don't Exit From this Fucntion Until user entered Enter Key */
		while(KEYPAD_getPressedKey() != ENTER_KEY){}

	}

}

/*
 * Description: Function to create the password and send to MC2 to check if the two passwords match.
 * Inputs: uint8* password, uint8* confirmed_password  "PASSWORD or CONFIRMED_PASSWORD"
 *		   that are saved in arrays
 * Returns: none
 */
void MC1_createPassword(){

	uint8 mismatch = TRUE;

	do{
		MC1_enterPassword(PASSWORD); /*Enter the pass 1st time and save it in Pass[PASSWORD_SIZE] */
		_delay_ms(500);
		MC1_enterPassword(CONFIRM_PASSWORD); /*Enter the pass 2nd time and save it in confimedPass[PASSWORD_SIZE] */
		/****SENDING THE 1st PASS TO MC2****/

		UART_sendByte(RECEIVE_PASS); /*check that the MC2 is ready to receive password*/

		while(UART_recieveByte() != SEND_PASS);  /*wait till MC2 is ready to Receive pass if not stay in the loop*/

		MC1_sendPassword(Pass); /*Send the 1st password to MC2*/

		/****SENDING THE 2nd PASS TO MC2****/

		UART_sendByte(RECEIVE_PASS); /*check that the MC2 is ready to receive password*/

		while(UART_recieveByte() != SEND_PASS);  /*wait till MC2 is ready to Receive pass if not stay in the loop*/

		MC1_sendPassword(confimedPass); /*Send the 2nd password to MC2*/

		uint8 i;
		i=UART_recieveByte();

		if(i == MISMATCH){
			mismatch = TRUE;
			LCD_clearScreen();
			LCD_displayString("ERROR MISMATCH");
			_delay_ms(1000);
		}
		else if(i == MATCH){
			mismatch = FALSE;
			LCD_clearScreen();
			LCD_displayString("DONE PASS SAVED");
			_delay_ms(1000);

		}
		LCD_clearScreen();
	}while(mismatch);

	/****PASSWORDS MATCHING****/  /****STORE PASSWORDS IN MC2 MODULE****/


}


/*
 * Description: Function to send the password the user inputs using uart to MC2.
 * Inputs: uint8* password
 * Returns: none
 */
void MC1_sendPassword(uint8* password){

	uint8 i;
	for(i=0; i<PASSWORD_SIZE ;i++){
		UART_sendByte(password[i]);  /*send the character i to MC2 using UART*/
		_delay_ms(50);
	}

}


/*
 * Description: Function to display Main Menu options.
 * Inputs: none
 * Returns: none
 */
void MC1_displayMainmenu(void){

	LCD_clearScreen();  /*clear lcd*/
	LCD_moveCursor(0, 0);
	LCD_displayString("+ : Open Door");  /*operation 1*/
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass");  /*operation 2*/


}


/*
 * Description:This is the call back function called by the Timer driver.
 * Inputs: none
 * Returns: none
 */
void MC1_incrementCounter(void){
	g_ticks++;
}


/*
 * Description: Function to display an Error Message when the user enter wrong password certain number of times
 * Inputs: none
 * Returns: none
 */
void MC1_warningMessage(void){

	g_ticks=0; /*reset the ticks to count 1 min from the start*/

	LCD_clearScreen();

	/*Display an Error message for 1 min*/
	LCD_displayString("ERROR Wrong Pass");
	while(g_ticks < 60){
	}


}


/*
 * Description: Function to display Door operations "LOCKING, UNLOCKING, LOCKED".
 * Inputs: none
 * Returns: none
 */
void MC1_displayDoorOperation(void){

	/**Door Opens**/
	g_ticks = 0;  /*reset the ticks to count 15 sec from the start*/
	LCD_clearScreen();  /*clear lcd*/
	LCD_displayString("Door Unlocking");  /*display the message for 15 sec*/
	while(g_ticks < DOOR_OPEN_TIME){
	}


	/**Door Held**/
	g_ticks = 0; /*reset the ticks to count 3 sec from the start*/
	LCD_clearScreen(); /*clear lcd*/
	LCD_displayString("Unlocked");  /*display the message for 15 sec*/
	while(g_ticks < DOOR_HOLD_TIME){
	}
	LCD_clearScreen(); /*clear lcd*/


	/**Door Closes**/
	g_ticks = 0;  /*reset the ticks to count 15 sec from the start*/
	LCD_clearScreen(); /*clear lcd*/
	LCD_displayString("Door Locking");  /*display the message for 15 sec*/

	while(g_ticks < DOOR_CLOSE_TIME){
	}
	LCD_clearScreen(); /*clear lcd for the next display*/

}

