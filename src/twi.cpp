/*
  twi.h - Simple TWI (I2C) library to perform I2C tasks without having to include wire.h or call wire.Begin in arduino code.
  Created by Jaap de Dood, July 11, 2019
*/

#include "Arduino.h"

void TWIInit(void)
{
    /* Set SCL frequency to ~200kHz */
    TWSR = 0x00;
    TWBR = 0x0C;
    /* Enable TWI */
    TWCR = (1<<TWEN);
}

void TWIStart(void)
{
    /* Send START condition */
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    /* Wait for TWINT Flag set. This indicates that the START condition has been transmitted */
    while(!(TWCR & (1<<TWINT)));
}

void TWIStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void TWIWrite(uint8_t u8data)
{
    /* Load data into TWDR Register. Clear TWINT bit in TWCR to start transmission */
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    /* Wait for TWINT Flag set. This indicates that the data has been transmitted, and ACK/NACK has been received. */
    while(!(TWCR & (1<<TWINT)));
}
