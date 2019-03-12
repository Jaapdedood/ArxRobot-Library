/*
  FuelGauge.cpp - Reads ADC DN, converts to voltage, and based on battery chemistry converts
  to a percentage, which is sent to the control panel. On undervoltage condition robot is
  place in a safe state.
  Created by Gary Hill, August 13, 2016.
*/

#include "Arduino.h"
#include "FuelGauge.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor

FuelGauge::FuelGauge(uint8_t id, int pin)
{
  _id = id;
  _pin = pin;
  pinMode(pin,INPUT);
}

/*
 * initialize battery properties to default values
 */
void FuelGauge::begin()      // initialize the packet
{
  setBatteyChem(LiPO);
}


/*
 *  Battery properties are defined by resistor divider and battery chemistry
 */
void  FuelGauge::setBatteyChem(Chem batt)
{
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  // Voltage Divider Network
  // Assuming common LiPO 3S design (highest voltage) for all battery types
    static const float R1 = 1000;    // 1K resistor from battery to analog input
    static const float R2 = 1000;    // 1K resistor from analog input to ground
    _K = (R2/(R1 + R2))*(1024/3.3);  // where DN = K * V (see reference documentation)
    // Vref = 3.3 +/- 0.1v and a 10-bit DAC
    // for LiPO 1S  K = 72.28235 and 1/K = 0.0138346
    if (batt == LiPO){   // all values converted to a 32-bit floating point digital number DN between 0.0 - 1024.0
        _V1 = 4.0*_K;      // volts/cell, 90% point on LiPO curve at 1C
        _V0 = 3.4*_K;      // volts/cell, 10% point on LiPO curve at 1C
    }
    else if (batt == NiMH){
        _V1 = 3*1.1875*_K; // volts/cell * 3 cells battery pack
        _V0 = 3*0.89*_K;   // volts/cell
    }
    else if (batt == LiFePO4){
        _V1 = 3.2*_K;      // volts/cell
        _V0 = 2.7*_K;      // volts/cell
    }
#else
    // Arduino UNO with variable resistor wired to 5V and GND with center tap to VBATT_PIN = A5
    _K = 1024/5.0;                   // K = 204.8
    _V1 = 1024;
    _V0 = 0;
#endif

    _dV = _V1 - _V0;
}

/*
 * simply return battery voltage as a floating point number
 */
 float FuelGauge::getVoltage()
 {
   float DN = (float) analogRead(_pin); // read adc and cast as float with 8-byte (64 bit) precision
   return DN/_K;                        // convert to voltage
 }

/*
 * Read the analog voltage DN and convert to a percentage based on
 * typical voltages for target battery when fully charged and depleted.
 * All variable units are DN.
 */
uint16_t FuelGauge::readFuelGauge()
{
  float V = (float) analogRead(_pin);    // read adc and cast as float with 8-byte (64 bit) precision
  V = 100*(V - _V0)/ _dV;                // convert to fuel gauge reading as a percentage (could use Arduino Map function)
  return (uint16_t) constrain(V,0,100);  // constrain to a floating point value between 0% and 100%
}


/*
 * Notes:
 */

// Optimization
// Translate to look-up table as defined for IR sensor in EE444
