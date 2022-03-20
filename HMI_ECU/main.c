/******************************************************************************
 *
 * File Name: main.c
 *
 * Description: MC1 code in Door Locker Security System project
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/

#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

/****************************** Definitions ******************************/
#define PASSED     0xFF
#define FAILED     0x00

/**************************** Global Variables ****************************/
volatile uint8 g_tick = 0;

/************************** Functions Prototypes **************************/
void getPassword();
void Door_processing();
void Buzzer_processing();

/******************************** Main ********************************/
int main()
{
	uint8 invalidCounter = 0, mainOptionsKey, password_status;

	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_BIT_PER_FRAME,DISABLE_PARITY,ONE_STOP_BIT,9600};

	/* Create configuration structure for TIMER driver with
	 * initial value = 0 & compare value = 7813
	 * which generates an interrupt every one second
	 */
	TIMER_ConfigType TIMER_Config = {TIMER1,TIMER_COMPARE_MODE,F_CPU_1024,0,7813};

	/* Initialize the UART driver */
	UART_init(&UART_Config);

	/* Initialize the LCD Driver */
	LCD_init();

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);


	while(1)
	{
		/******************** First Step ******************************/

		if(invalidCounter != 3)
		{
			/* Stay in this loop until the password is correct */
			do{
				LCD_clearScreen();

				LCD_displayString("Enter your new password :");
				LCD_moveCursor(1, 0);

				getPassword();

				LCD_clearScreen();

				LCD_displayString("Re-enter password :");
				LCD_moveCursor(1, 0);

				getPassword();

				password_status = UART_recieveByte();
				LCD_clearScreen();

				if(password_status == FAILED)
				{
					LCD_moveCursor(0, 0);
					LCD_displayString("Passwords don't match");
					_delay_ms(500);
				}

			}while(password_status == FAILED);
		}

/****************************** Step 2 ******************************/

		invalidCounter = 0;

		/* Go to Main options */
		LCD_clearScreen();

		LCD_displayString("Press '+' for openning the door");
		LCD_moveCursor(1, 0);
		LCD_displayString("Press '-' for changing the password");

		/* Take the pressed key until it's equal to '+' or '-' */
		do{
			mainOptionsKey = KEYPAD_getPressedKey();

			/* Press time */
			_delay_ms(300);

		}while((mainOptionsKey != '+') && (mainOptionsKey != '-'));

		/* Send the key number to the second MC */
		UART_sendByte(mainOptionsKey);

		/* Take the password */
		do{
			/* If the password entered is wrong */
			if(invalidCounter > 0)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(700);
			}
			LCD_clearScreen();

			LCD_displayString("Please enter password :");
			LCD_moveCursor(1, 0);

			getPassword();

			password_status = UART_recieveByte();

			invalidCounter++;

		}while((password_status == FAILED) && (invalidCounter < 3));

		/* Send any default data to MC2 to initialize the
		 * timer with it at the same time
		 */
		UART_sendByte(mainOptionsKey);

/****************************** Step 3 ******************************/

		/* If the input password is wrong for three trials */
		if(invalidCounter == 3)
		{
			/* Set the call back function */
			TIMER_setCallBack(Buzzer_processing);

			/* Initialize the TIMER Driver */
			TIMER_Init(&TIMER_Config);

			/* Waiting for one minute */
			while(g_tick != 60){}

			g_tick = 0;
		}

		/* If the chosen key is to open the door */
		if((mainOptionsKey == '+') && (invalidCounter < 3))
		{

			/* Set the call back function */
			TIMER_setCallBack(Door_processing);

			/* Initialize the TIMER Driver */
			TIMER_Init(&TIMER_Config);

			/* Wait Until the door is unlocked then locked again */
			while(g_tick != 33){}

			g_tick = 0;

			/* Make the error counter equals 3
			 * to jump into the main options
			 */
			invalidCounter = 3;
		}

	}
}

/************************** Functions Definitions **************************/
/* Function of recieving the password from the keypad */
void getPassword()
{
	uint8 i=0, key;
	for(i=0; i<5; i++)
	{
		/* Get the pressed key */
		key = KEYPAD_getPressedKey();

		/* Send the key number to the second MC */
		UART_sendByte(key);

		/* Show '*' on the LCD */
		LCD_displayCharacter('*');

		/* Press time */
		_delay_ms(300);
	}
}

/* Call back function of door processing */
void Door_processing()
{
	g_tick++;

	/* After one second */
	if(g_tick == 1)
	{
		LCD_clearScreen();
		/* Display on the LCD "The Door is Unlocking" */
		LCD_displayString("The Door is Unlocking...");
	}

	/* After 18 seconds */
	else if(g_tick == 18)
	{
		LCD_clearScreen();
		/* Display on the LCD "The Door is Locking" */
		LCD_displayString("The Door is Locking...");
	}

	/*After 33 seconds */
	else if(g_tick == 33)
	{
		/* Clear the Screen */
		LCD_clearScreen();

		/*Stop the timer again */
		TIMER_DeInit();

	}
}

/* Call back function of buzzer processing */
void Buzzer_processing()
{
	g_tick++;

	/* After one second */
	if(g_tick == 1)
	{
		LCD_clearScreen();
		/* Display an error message on the LCD */
		LCD_displayString("Do you have Alzheimer's or what!!");
	}

	/* After one minute */
	if(g_tick == 60)
	{
		/* Clear the Screen */
		LCD_clearScreen();

		/*Stop the timer */
		TIMER_DeInit();
	}
}
