/*
  TB6612FNG.h - TB6612FNG H-Bridge
  Created by Gary Hill, August 13, 2016.
*/
#ifndef TB6612FNG_h      // The #define Guard
#define TB6612FNG_h

#include "Arduino.h"     // Arduino library files
#include "Configure.h"
#include <Motor.h>

/*
 *  header .h class member definitions
 */

class TB6612FNG
{
public:
    // Constructor
    TB6612FNG();

    // Public class 'methods'
    void begin();
    void motors_go(uint8_t*);
    void motors_stop();
    void motors_safe();

private:
    // Private class 'methods'

    // Private class 'properties'

    // Instantiate Motor Classes
    Motor motorA;
    Motor motorB;

}; // end of class definition

#endif

/* TB6612FNG Control Functions
     Source: H-SW Control Functions found in the Datasheet on Page 4
             https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf
     Pin#  = IN1    IN2
     Logic = inPin1 inPin2  Mode
              0     0       OFF (High Impedance)
              0     1       BACKWARD (CCW)
              1     0       FORWARD  (CW)
              1     1       Short brake
*/
