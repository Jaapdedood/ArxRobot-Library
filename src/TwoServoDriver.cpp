/*
  Created by Patrik Hertle, March 27, 2019
  Modified by Jaap de Dood for ArxRobot Library
*/

#include "Arduino.h"
#include "Configure.h"
#include "ServoDriver.h"
#include "TwoServoDriver.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor

TwoServoDriver::TwoServoDriver()
{
}


/*
 * initialize servo properties to default values
 */
void TwoServoDriver::begin()
{
    pinMode(STBY,OUTPUT); // TODO: Is this necessary?
    this->servoA.begin(SERVO_A);
    this->servoB.begin(SERVO_B);

    servos_reset();                     // servos at 0 Degree
}


/*

 */
void TwoServoDriver::servos_go(uint8_t* servodata)
{
    cli();
    angleA = (servodata[3] << 8) | servodata[4];
    angleB = (servodata[5] << 8) | servodata[6];

    servoA.setAngle(angleA);
    servoB.setAngle(angleB);
    sei();
}


/*
 * Set the servos in a reset angle
 */
void TwoServoDriver::servos_reset()
{
    servoA.setAngle(0);
    servoB.setAngle(0);

}



/*
 * Set the servos in the home defined angle
 */
void TwoServoDriver::servos_home()
{
    servoA.setAngle(90);
    servoB.setAngle(90);

}
