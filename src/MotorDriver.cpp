/*
  MotorDriver.cpp
  Created by Jaap de Dood, January 9 2020. 
  Based on "TB6612FNG.cpp" by Gary Hill
*/

#include "Arduino.h"
#include "Configure.h"
#include "Motor.h"
#include "MotorDriver.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor

MotorDriver::MotorDriver()
{
}

void MotorDriver::begin()
{
#if defined(ARDUINO_AVR_3DOT)
    pinMode(STBY,OUTPUT);
    this->motorA.begin(AIN1, AIN2, PWMA);
    this->motorB.begin(BIN1, BIN2, PWMB);
    motors_stop();                     // motors off
#else
    pinMode(STBY,OUTPUT);
    this->motorA.begin(AIN1, AIN2);
    this->motorB.begin(BIN1, BIN2);
    motors_stop();                     // motors off
#endif
    digitalWrite(STBY, HIGH);        // MotorDriver enabled
}

void MotorDriver::motors_go(uint8_t* motordata)
{
    motorA.go(motordata[3],motordata[4]);  // configure H-bridge & speed for motor A (left)
    motorB.go(motordata[5],motordata[6]);  // configure H-bridge for motor B (right)
}

void MotorDriver::motors_stop()        // initialize or stop MotorDriver
{
    motorA.brake();
    motorB.brake();
}

void MotorDriver::motors_safe()        // initialize or stop MotorDriver
{
    motors_stop();                     // motors off
    digitalWrite(STBY, LOW);           // MotorDriver enabled
}
