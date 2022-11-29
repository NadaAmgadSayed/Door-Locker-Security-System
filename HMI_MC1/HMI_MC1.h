/*
 * HMI_MC1.h
 *
 *  Created on: Nov 6, 2022
 *      Author: Nada Amgad
 */

#ifndef HMI_MC1_H_
#define HMI_MC1_H_

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>

#define PASSWORD_SIZE       5 /*set the size of the password to 5 statically*/
#define ENTER_KEY          13
#define DOOR_OPEN_TIME     15
#define DOOR_CLOSE_TIME    15
#define DOOR_HOLD_TIME      3

/*BOOLEANS*/
#define TRUE                1
#define FALSE               0

/*UART COMMUNICATION*/
#define RECEIVE_PASS       10
#define SEND_PASS          20
#define MISMATCH            0
#define MATCH               1
#define STORE_PASS         30
#define PASS_STORED        40
#define DOOR_UNLOCKING     50
#define CHANGE_PASSWORD    60
#define OPEN_DOOR          70
#define CHANGE_PASS        80

typedef enum{
	PASSWORD ,CONFIRM_PASSWORD
}password_status;


/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/

extern uint8 Pass[PASSWORD_SIZE], confimedPass[PASSWORD_SIZE];


/*******************************************************************************
 *                      Functions Prototypes                                  *
 *******************************************************************************/


/*
 * Description: Function to take the password input from the user.
 * Inputs: uint8 status  "PASSWORD or CONFIRM_PASSWORD" to display the correct commands
 * Returns: none
 */
void MC1_enterPassword(uint8 status);

/*
 * Description: Function to create the password and send to MC2 to check if the two passwords match.
 * Inputs: uint8* password, uint8* confirmed_password  "PASSWORD or CONFIRMED_PASSWORD"
 *		   that are saved in arrays
 * Returns: none
 */
void MC1_createPassword(/*uint8* password, uint8* confirmed_password*/);

/*
 * Description: Function to send the password the user inputs using uart to MC2.
 * Inputs: uint8* password
 * Returns: none
 */
void MC1_sendPassword(uint8* password);

/*
 * Description: Function to display Main Menu options.
 * Inputs: none
 * Returns: none
 */
void MC1_displayMainmenu(void);

/*
 * Description:This is the call back function called by the Timer driver.
 * Inputs: none
 * Returns: none
 */
void MC1_incrementCounter(void);

/*
 * Description: Function to display an Error Message when the user enter wrong password certain number of times
 * Inputs: none
 * Returns: none
 */
void MC1_warningMessage(void);


/*
 * Description: Function to display Door operations "LOCKING, UNLOCKING, LOCKED".
 * Inputs: none
 * Returns: none
 */
void MC1_displayDoorOperation(void);




#endif /* HMI_MC1_H_ */
