/*
 * ServoDriver.cpp - Library for PanAndTiltDriver
 * Created by Patrik Hertle, March 27, 2019
 * Modified by Jaap de Dood for ArxRobot Library
 */

#include "Arduino.h"
#include <Servo.h>
#include "ServoDriver.h"


/*
 *  C++ .cpp member class definitions
 */

// Constructor
ServoDriver::ServoDriver()
{

}

/*
 * initialize servo properties to default values
 */
void ServoDriver::begin(uint8_t pin)   // initialize the packet
{
  _pin = pin;
  myservo.attach(pin);

  angle = 0;	//Define angle at first time code running;
}


/*
 * Checked first if the angle is in the range and than set the Servo to the angle.
 */
void ServoDriver::setAngle(uint16_t pos)
{
   angle = pos;
   if (angle > maxAngle)
   {
    angle = maxAngle;
   }
   else if(angle < minAngle)
   {
    angle = minAngle;
   }
   else
   {
    angle = pos;
   }

   myservo.write(angle);

}
