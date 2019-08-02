/*
  Created by Jaap de Dood, July 24, 2019
*/
#ifndef ServoDriver_h      // The #define Guard
#define ServoDriver_h

#include "Arduino.h"     // Arduino library files
#include "Configure.h"
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
    void begin(uint8_t*, uint8_t*, uint8_t*, uint8_t);
    void servos_go(uint8_t*, uint8_t);
    void servos_reset(uint8_t);
    void servos_home(uint8_t*, uint8_t);
private:
    // Private class 'methods'

    // Private class 'properties'

    // Instantiate Servo Classes
    Servo myServos[];
    //uint16_t angles[];

}; // end of class definition

#endif
