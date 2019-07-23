/*
  Created by Patrik Hertle, March 27, 2019
  Modified by Jaap de Dood for ArxRobot Library
*/
#ifndef TwoServoDriver_h      // The #define Guard
#define TwoServoDriver_h

#include "Arduino.h"     // Arduino library files
#include "Configure.h"
#include "ServoDriver.h"

/*
 *  header .h class member definitions
 */

class TwoServoDriver
{
public:
    // Constructor
    TwoServoDriver();

    // Public class 'methods'
    void begin();
    void servos_go(uint8_t*);
    void servos_reset();
    void servos_home();


private:
    // Private class 'methods'

    // Private class 'properties'

    // Instantiate Motor Classes
    // TODO: should these be combined to a single array?
    ServoDriver servoA;
    ServoDriver servoB;
    uint16_t angleA;
    uint16_t angleB;

}; // end of class definition

#endif
