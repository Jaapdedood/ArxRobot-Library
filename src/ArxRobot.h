/*
  ArxRobot.h - Base Class
*/

#ifndef ArxRobot_h    // The #define Guard
#define ArxRobot_h

// extern const uint8_t CMD_LIST_SIZE;
#include "Arduino.h"       // Arduino library files
#include <avr/wdt.h>       // Standard C library for AVR-GCC avr-libc wdt.h
#include "TelecomClass.h"  // DRV8848 Motor Driver
#include "servo3DoT.h"
#include "twi.h"
#include "Configure.h"

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
    void addCustomCommand(fptr_t, uint8_t);
    // void addToBuiltInCommand(uint8_t, fptr_t);
    void replaceBuiltInCommand(uint8_t, fptr_t);

    uint16_t readBatteryVoltage(); // TODO move these to appropriate class. Placed here since Fuelgauge class may be rebuilt soon.
    void alertFatalError();        // TODO Same as readBatteryVoltage, + incorporate telecom data to app

    void debugFunction();

private:
    // Private class 'methods'
    void _clear_MCUCR_JTD_bit();
    void commandProcessor();



  // Private class 'properties'
    cmdFunc_t _customCommands[MAX_CUSTOM_COMMANDS(CUSTOM_COMMANDS_START)]; //0x40 to 0x5F = 31 max commands
    cmdFunc_t _interceptCommands[BUILTIN_COMMANDS_COUNT];
};

// create an instance of ArxRobot class
// extern ArxRobot ArxRobot;

//extern SerialBridgeClass Bridge;
#endif  // ArxRobot_h
