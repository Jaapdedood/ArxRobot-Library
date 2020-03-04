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
void Motor::begin(uint8_t pin1, uint8_t pin2)   // initialize the packet
{
  _pin1 = pin1;
  _pin2 = pin2;
#if BOARD_VERSION == 0x00
  _pinPWM = pinPWM;
  pinMode(pinPWM,OUTPUT);
#endif
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  }

void Motor::brake()
{
  digitalWrite(_pin1, HIGH);  // short brake
  digitalWrite(_pin2, HIGH);
}

// direction: FORWARD = index 1, BACKWARD = index 2, BRAKE = index 3, RELEASE = index 4
void Motor::go(uint8_t dir, uint8_t pwm)
{
#if BOARD_VERSION == 0x00
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
#else
  switch (dir)  // set dir = motordata[1] for motor A (left)
  {
    case 1:  // FORWARD
      analogWrite(_pin1, pwm);
      analogWrite(_pin2, 0);
      break;
    case 2:  // BACKWARD
      analogWrite(_pin1, 0);
      analogWrite(_pin2, pwm);
      break;
    case 3:  // BRAKE
      analogWrite(_pin1, 255);
      analogWrite(_pin2, 255);
      break;
    case 4:  // RELEASE
      analogWrite(_pin1, 0);
      analogWrite(_pin2, 0);
      break;
    default:
      analogWrite(_pin1, 0);
      analogWrite(_pin2, 0);
  }
#endif
}

/* DRV8848 Truth Table:
    xIN1    xIN2    xOUT1   xOUT2   Function
    0       0       Z       Z       Coast (fast decay)
    0       1       L       H       Reverse
    1       0       H       L       Forward
    1       1       L       L       Brake (slow decay)

    Note: Pins AIN1 and AIN2 are tri-level, so when they are left Hi-Z, they are not internally pulled to logic low.
    When AIN1 or AIN2 are set to Hi-z and not in parallel mode, the output driver maintains the previous state.
*/
