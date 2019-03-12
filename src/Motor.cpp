/*
 * Motor.cpp - Library for TB6612FNG H-Bridge
 * Created by Gary Hill, July 13, 2016
 */

#include "Arduino.h"
#include "Motor.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor
Motor::Motor()
{
}

/*
 * initialize motor properties to default values
 */
void Motor::begin(uint8_t pin1, uint8_t pin2, uint8_t pinPWM)   // initialize the packet
{
  _pin1 = pin1;
  _pin2 = pin2;
  _pinPWM = pinPWM;
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pinPWM,OUTPUT);
}

void Motor::brake()
{
  digitalWrite(_pin1, HIGH);  // short brake
  digitalWrite(_pin2, HIGH);
}

// direction: FORWARD = index 1, BACKWARD = index 2, BRAKE = index 3, RELEASE = index 4
void Motor::go(uint8_t dir, uint8_t pwm)
{
  bool out1, out2;

  switch (dir)  // set dir = motordata[1] for motor A (left)
  {
    case 1:  // FORWARD
      out1 = HIGH;
      out2 = LOW;
      break;
    case 2:  // BACKWARD
      out1 = LOW;
      out2 = HIGH;
      break;
    case 3:  // BRAKE
      out1 = HIGH;
      out2 = HIGH;
      break;
    case 4:  // RELEASE
      out1 = LOW;
      out2 = LOW;
      break;
    default:
      out1 = LOW;
      out2 = LOW;
  }
  digitalWrite(_pin1, out1);
  digitalWrite(_pin2, out2);
  analogWrite(_pinPWM, pwm);          // set speed motor A (left motor)
}
