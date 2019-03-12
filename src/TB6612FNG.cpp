/*
  TB6612FNG.cpp - TB6612FNG H-Bridge
  Created by Gary Hill, August 13, 2016.
*/

#include "Arduino.h"
#include "Configure.h"
#include "Motor.h"
#include "TB6612FNG.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor

TB6612FNG::TB6612FNG()
{
}

void TB6612FNG::begin()
{
    pinMode(STBY,OUTPUT);
    this->motorA.begin(AIN1, AIN2, PWMA);
    this->motorB.begin(BIN1, BIN2, PWMB);
    motors_stop();                     // motors off
    digitalWrite(STBY, HIGH);          // TB6612FNG enabled
}

void TB6612FNG::motors_go(uint8_t* motordata)
{
    motorA.go(motordata[3],motordata[4]);  // configure H-bridge & speed for motor A (left)
    motorB.go(motordata[5],motordata[6]);  // configure H-bridge for motor B (right)
    analogWrite(PWMB, motordata[6]);       // set speed motor B (right motor)
}

void TB6612FNG::motors_stop()        // initialize or stop TB6612FNG
{
    motorA.brake();
    motorB.brake();
}

void TB6612FNG::motors_safe()        // initialize or stop TB6612FNG
{
    motors_stop();                     // motors off
    digitalWrite(STBY, LOW);           // TB6612FNG enabled
}
