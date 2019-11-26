/*
  ArxRobot.h - Base Class
  Created by Gary Hill, August 13, 2016.
*/

#ifndef ArxRobot_h    // The #define Guard
#define ArxRobot_h

// extern const uint8_t CMD_LIST_SIZE;
#include "Arduino.h"       // Arduino library files
#include <avr/wdt.h>       // Standard C library for AVR-GCC avr-libc wdt.h
#include "TelecomClass.h"  // TB6612FNG Motor Driver
#include "servo3DoT.h"
#include "twi.h"

/*
 *  header .h class member definitions
 */

class ArxRobot
{
public:
    // Constructor
    ArxRobot();

    // Data type and structure descriptions
    typedef void (*fptr_t) (const uint8_t, uint8_t *, uint8_t);
    // Now that we have a type which can point to a function (fptr), we can make an instance
    // of that type (in other words, a variable that points to a function)
    // fptr_t anotherFunction;

    struct cmdFunc_t {
        uint8_t  cmd;
        fptr_t   funct;
    };

    // Public class 'methods'
    void begin();
    void loop();
    void setOnCommand(cmdFunc_t*,uint8_t);
    void setCurrentLimit(uint8_t);

private:
    // Private class 'methods'
    void _clear_MCUCR_JTD_bit();
    void commandProcessor();
    uint8_t search_onCommand(uint8_t);

  // Private class 'properties'
  static cmdFunc_t* _onCommand;
  uint8_t _arraysize;

};

// create an instance of ArxRobot class
// extern ArxRobot ArxRobot;

//extern SerialBridgeClass Bridge;
#endif  // ArxRobot_h
