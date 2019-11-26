/*
  FuelGauge.h - Reads ADC DN, converts to voltage, and based on battery chemistry converts
  to a percentage, which is sent to the control panel. On undervoltage condition robot is
  place in a safe state.
  Created by Gary Hill, August 13, 2016
*/
#ifndef FuelGauge_h      // The #define Guard
#define FuelGauge_h

#include "Arduino.h"     // Arduino library files

enum Chem {LiPO, NiMH, LiFePO4};

/*
 * header class member definitions
 */

class FuelGauge
{
  public:

    // Constructor
  FuelGauge(uint8_t id, int pin);

    // Public class 'methods'
  void begin();
  uint16_t readFuelGauge();
  void setBatteyChem(Chem);
  float getVoltage();

  private:

  // Private class 'methods'

  // Private class 'properties'
  uint8_t _id;
  int    _pin;
  float  _K, _V1, _V0, _dV;  // defined by resistor divider and battery chemistry

}; // end of class definition

#endif
