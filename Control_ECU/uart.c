 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;

	/************************** UCSRA Description **************************/
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRB |= ((Config_Ptr->dataBits) & 0x04);

	/************************** UCSRC Description **************************/
	UCSRC = (1<<URSEL);
	UCSRC |= ((Config_Ptr->parityType) << 4);
	UCSRC |= ((Config_Ptr->stopBit) << 3);
	UCSRC |= (((Config_Ptr->dataBits) & 0x03) << 1);
	
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((Config_Ptr->baudRate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH */
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}


void UART_sendByte(const uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE));

	UDR = data;

}


uint8 UART_recieveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

    return UDR;		
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
