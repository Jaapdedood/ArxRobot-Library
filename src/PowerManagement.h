/*
  PowerManagement.h - Formerly FuelGauge.h Reads ADC DN, converts to voltage, and based on battery chemistry converts
  to a percentage, which is sent to the control panel. On undervoltage condition robot is
  place in a safe state.
  Contains functions for current limiting using TPS2553 and MCP4017
  Created by Gary Hill, August 13, 2016
  Edited by Jaap de Dood, July 11, 2019
*/
#ifndef PowerManagement_h      // The #define Guard
#define PowerManagement_h

#include "Arduino.h"     // Arduino library files
#include "twi.h"         // I2C functions

enum Chem {LiPO, NiMH, LiFePO4};

/*
 * header class member definitions
 */

class PowerManagement
{
public:

    // Constructor
    PowerManagement(uint8_t id, int pin);

    // Public class 'methods'
    void begin();
    uint16_t readFuelGauge();
    void setBatteyChem(Chem);
    float getVoltage();
    void setCurrentLimit(uint8_t);

private:
    // Private class 'methods'

    // Private class 'properties'
    uint8_t _id;
    int    _pin;
    float  _K, _V1, _V0, _dV;  // defined by resistor divider and battery chemistry

}; // end of class definition

#endif
