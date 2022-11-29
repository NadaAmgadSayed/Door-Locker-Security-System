/******************************************************************************
 *
 * Module: TIMER 1
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR timer 1 driver
 *
 * Author: Nada Amgad
 *
 *******************************************************************************/


#include "timer.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use Timer1 Registers */
#include <avr/interrupt.h> /* For ISR */



/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}


ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description: Function to initialize the Timer driver
 * Inputs: pointer to the configuration structure with type Timer1_ConfigType.
 * Return: None
 *
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr){

	TCNT1 = Config_Ptr -> initial_value;

	if((Config_Ptr -> mode) == NORMAL_MODE){

		TIMSK |= (1<<TOIE1); /* Enable Timer1 Compare A Interrupt */
		TCCR1A = (1<<FOC1A);
		TCCR1B = (Config_Ptr->prescaler);
	}
	else if((Config_Ptr -> mode) == COMPARE_MODE){

		OCR1A = Config_Ptr-> compare_value;
		TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
		TCCR1A = (1<<FOC1A);
		TCCR1B = (1<<WGM12) | (Config_Ptr->prescaler);

	}

}


/*
 * Description: Function to set the Call Back function address.
 * Inputs:  pointer to Call Back function
 * Return: None
 *
 */
void Timer1_setCallBack(void(*a_ptr)(void)){

	g_callBackPtr = a_ptr ;
}

/*
 * Description: Function to disable the Timer1.
 * Inputs: None
 * Return: None
 *
 */
void Timer1_deInit(void){

//	CLEAR_BIT(TCCR1B , CS12);
//	CLEAR_BIT(TCCR1B , CS11);
//	CLEAR_BIT(TCCR1B , CS10);

	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	/*for compare mode*/
	TIMSK &= ~(1<<OCIE1A);

	/*for overflow mode*/
	TIMSK &= ~(1<<TOIE1);

}
