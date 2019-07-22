/*
  twi.h - Simple TWI (I2C) library to perform I2C tasks without having to include wire.h or call wire.Begin in arduino code.
  Created by Jaap de Dood, July 11, 2019
*/
#ifndef twi_h
#define twi_h

#define SLA_W 0x5E // MCP4017 Slave address + write bit.

#include "Arduino.h"

void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t);

#endif
