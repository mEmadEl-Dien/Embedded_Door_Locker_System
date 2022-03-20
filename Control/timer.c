 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/

#include "timer.h"
#include "avr/io.h" /* To use the TIMER Registers */
#include <avr/interrupt.h> /* For TIMER ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/************************ TIMER0 ************************/
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

/************************ TIMER1 ************************/
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

/************************ TIMER2 ************************/
ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void TIMER_Init(const TIMER_ConfigType * Config_Ptr)
{
	/************************ TIMER0 ************************/
	if(Config_Ptr->id == TIMER0)
	{
		if(Config_Ptr->mode == TIMER_NORMAL_MODE)
		{
			TCCR0 = (1<<FOC0);

			/* Enable Timer0 Overflow Interrupt */
			TIMSK |= (1<<TOIE0);
		}

		else if(Config_Ptr->mode == TIMER_COMPARE_MODE)
		{
			TCCR0 = (1<<FOC0) | (1<<WGM01);

			/* Enable Timer0 Compare Interrupt */
			TIMSK |= (1<<OCIE0);

			/* Set Timer0 Compare Value */
			OCR0 = (Config_Ptr->compareValue);
		}
		TCCR0 |= (Config_Ptr->presaler);

		/* Set Timer0 Initial Value */
		TCNT0 = (Config_Ptr->initialValue);
	}

	/************************ TIMER1 ************************/

	if(Config_Ptr->id == TIMER1)
	{
		if(Config_Ptr->mode == TIMER_NORMAL_MODE)
		{
			TCCR1A = (1<<FOC1A) | (1<<WGM10) | (1<<WGM11);
			TCCR1B = 0;
			TIMSK |= (1<<TOIE1);
		}

		else if(Config_Ptr->mode == TIMER_COMPARE_MODE)
		{
			TCCR1A = (1<<FOC1A);
			TCCR1B = (1<<WGM12);
			TIMSK |= (1<<OCIE1A);
			OCR1A = (Config_Ptr->compareValue);
		}

		TCCR1B |= (Config_Ptr->presaler);
		TCNT1 = (Config_Ptr->initialValue);
	}

	/************************ TIMER2 ************************/

	if(Config_Ptr->id == TIMER2)
	{
		if(Config_Ptr->mode == TIMER_NORMAL_MODE)
		{
			TCCR2 = (1<<FOC2);
			TIMSK |= (1<<TOIE2);
		}

		else if(Config_Ptr->mode == TIMER_COMPARE_MODE)
		{
			TCCR2 = (1<<FOC2) | (1<<WGM21);
			TIMSK |= (1<<OCIE2);
			OCR2 = (Config_Ptr->compareValue);
		}

		TCCR2 |= (Config_Ptr->presaler);
		TCNT2 = (Config_Ptr->initialValue);
	}
}

void TIMER_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

void TIMER_DeInit(void)
{
	/************************ TIMER0 ************************/

	/* Clear All Timer0 Registers and stop the clock */
	TCCR0 = 0;
	TCNT0 = 0;

	/* Disable Timer0 Overflow Interrupt */
	TIMSK &= ~(1<<TOIE0);

	/* Disable Timer0 Compare Interrupt */
	TIMSK &= ~(1<<OCIE0);

	/************************ TIMER1 ************************/

	/* Clear All Timer1 Registers and stop the clock */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	/* Disable Timer1 Overflow Interrupt */
	TIMSK &= ~(1<<TOIE1);

	/* Disable Timer1 Compare Interrupt */
	TIMSK &= ~(1<<OCIE1A);

	/************************ TIMER2 ************************/

	/* Clear All Timer2 Registers and stop the clock */
	TCCR2 = 0;
	TCNT2 = 0;

	/* Disable Timer2 Overflow Interrupt */
	TIMSK &= ~(1<<TOIE2);

	/* Disable Timer2 Compare Interrupt */
	TIMSK &= ~(1<<OCIE2);
}
