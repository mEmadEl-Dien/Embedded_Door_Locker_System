 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Emad El-Dien
 *
 *******************************************************************************/
 
#include "twi.h"

#include "common_macros.h"
#include <avr/io.h>

void TWI_init(uint32 bit_rate, uint8 address)
{
	/* Bit Rate using zero pre-scaler TWPS=00 */
	TWBR = (((F_CPU / bit_rate) - 16) / 2);
	TWSR = 0x00;
	
    /* Two Wire Bus address my address if any master device want to call me */
    TWAR = address;
	
    TWCR = (1<<TWEN); /* enable TWI */
}

void TWI_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(BIT_IS_CLEAR(TWCR,TWINT));
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;

    status = TWSR & 0xF8;
    return status;
}
