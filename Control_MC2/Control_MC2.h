/*
 * Control_MC2.h
 *
 *  Created on: Nov 6, 2022
 *      Author: Nada Amgad
 */

#ifndef CONTROL_MC2_H_
#define CONTROL_MC2_H_

#include "dc_motor.h"
#include "twi.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "uart.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>

#define PASSWORD_SIZE       5
#define SAVE_ADDRESS    0x311
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



/*******************************************************************************
 *                      Functions Prototypes                                  *
 *******************************************************************************/

/*
 * Description: Function to receive the password the user inputs using uart from MC1.
 * Inputs: uint8* password
 * Returns: none
 */
void MC2_receivePassword(uint8* receivedPassword);

/*
 * Description:Function to compare passwords and check correctness
 * Inputs: uint8* password, uint8* confirmed_password that are saved in arrays
 * Returns: True or False "0 or 1" of type uint8
 */
uint8 MC2_checkPasswords(uint8* Password ,uint8* confirmPassword);

/*
 * Description:Function to verify passwords and communicate with uart MC1 then saves the correct pass in the EEPROM
 * Inputs: none
 * Returns: none
 */
void MC2_verifyPasswords(void);

/*
 * Description:This is the call back function called by the Timer driver.
 * Inputs: none
 * Returns: none
 */
void MC2_incrementCounter(void);

/*
 * Description: Function to store the password in the EEPROM in a specified save address
 * Inputs: uint8* password  "Password that the user inputs and confirmed in array"
 * Returns: none
 */
void MC2_savePassword(uint8* password);

/*
 * Description: Function to get the saved password in the EEPROM in a specified save address
 * Inputs: uint8* password  "Array in witch the password will be returned from the EEPROM"
 * Returns: none
 */
void MC2_getSavedPassword(uint8* password);

/*
 * Description: Function to Manage Door operations "LOCKING, UNLOCKING, LOCKED".
 * Inputs: none
 * Returns: none
 */
void MC2_manageDoorOperation(void);

/*
 * Description: Function to activate buzzer when the user enter wrong password certain number of times
 * Inputs: none
 * Returns: none
 */
void MC2_warningState(void);

#endif /* CONTROL_MC2_H_ */
