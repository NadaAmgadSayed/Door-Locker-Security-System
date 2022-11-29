/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the buzzer driver
 *
 * Author: Nada Amgad
 *
 *******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


#define BUZZER_PORT_ID    PORTC_ID
#define BUZZER_PIN_ID     PIN7_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/



/*
 *
 *Description:
 *Setup the direction for the buzzer pin as output pin through the GPIO driver.
 *Turn off the buzzer through the GPIO.
 *Inputs: None
 *Return: None
 *
 */
void Buzzer_init();

/*
 *
 *Description:
 *Function to enable the Buzzer through the GPIO.
 *Inputs: None
 *Return: None
 *
 */
void Buzzer_on(void);

/*
 *
 *Description:
 *Function to disable the Buzzer through the GPIO.
 *Inputs: None
 *Return: None
 *
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
