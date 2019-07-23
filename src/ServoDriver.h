/*
 * ServoDriver.h - for PanAndTiltDriver
 * Patrik Hertle, March 27, 2019
 * Modified by Jaap de Dood for ArxRobot Library
 */
#ifndef ServoDriver_h          // The #define Guard
#define ServoDriver_h

#include "Arduino.h"     // Arduino library files
#include <Servo.h>


/*
 *  header .h class member definitions
 */

class ServoDriver
{
public:
    // Constructor
    ServoDriver();

    // Public class 'methods'
    void begin(uint8_t);		//act like a Constructor
    void setAngle(uint16_t);		//drives the Servo to given angle


private:

    // Private class 'methods'

    // Private class 'properties'
    uint8_t _pin;
    uint16_t angle;	// TRUE = Unloaded and FALSE = Not Unloaded
    Servo myservo;
    uint16_t const maxAngle = 360;
    uint16_t const minAngle = 0;

}; // end of class definition

#endif
