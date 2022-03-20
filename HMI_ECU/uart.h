 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	FIVE_BIT_PER_FRAME,
	SIX_BIT_PER_FRAME,
	SEVEN_BIT_PER_FRAME,
	EIGHT_BIT_PER_FRAME,
	NINE_BIT_PER_FRAME=7
}UART_dataBits;

typedef enum
{
	DISABLE_PARITY,
	EVEN_PARITY=2,
	ODD_PARITY
}UART_ParityType;

typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BIT
}UART_StopBit;

typedef struct
{
	UART_dataBits dataBits;
	UART_ParityType parityType;
	UART_StopBit stopBit;
	uint32 baudRate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
