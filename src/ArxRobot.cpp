/*
  ArxRobot.cpp - ArxRobot Base Class
  Created by Gary Hill, August 13, 2016
  Edited by Jaap de Dood & Jeff Gomes, March 2019
*/

#include "Arduino.h"       // Arduino library files
#include <avr/wdt.h>       // Standard C library for AVR-GCC avr-libc wdt.h
#include "TelecomClass.h"  // TB6612FNG Motor Driver
#include "ArxRobot.h"


// robotControl model included an extern preprocessor directive.

/*==============================================================================
 * MACROS
 *============================================================================*/

/* shortcut for setFirmwareNameAndVersion() that uses __FILE__ to set the
 * firmware name.  It needs to be a macro so that __FILE__ is included in the
 * firmware source file rather than the library source file.
 */
#define setFirmwareVersion(x, y)   setFirmwareNameAndVersion(__FILE__, x, y)

/*==============================================================================
 * Instantiate Objects
 *============================================================================*/

// create an instance of a packet to send the value from the Output Compare Register Timer 4D wired to PWM signal to Motor B
TelecomClass telecom;

/*
 *  C++ .cpp member class definitions
 */

// Constructor
ArxRobot::ArxRobot()  // based on Firmata.cpp constructor
{
  /*
   * Firmata (.h and .cpp) have implemented version control ****
   * TODO Port to 3DoT
   * firmwareVersionCount = 0;
   * firmwareVersionVector = 0;
   * systemReset();
   */
}

/*
 * See data type and structure specifier in header for declaration of cmdFunc_t
 * as a 2 dimensional array of commands and associated user defined
 * subroutines.
 * Set _onCommand property a pointer type cmdFunc_t* to NULL
 */
ArxRobot::cmdFunc_t* ArxRobot::_onCommand = NULL;

/*********************
 *  Public Methods   *
 *********************/

/*
 * initialize ArxRobot properties to default values
 */
void ArxRobot::begin()
{
  wdt_disable();               // the watchdog timer remains active even after a system reset (except a power-on condition)
                               // hopefully Arduino bootloader does this, because by now it is probably to late to prevent
                               // an eternal loop of time-out resets.

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  //Code in here will only be compiled if an Arduino Leonardo is used.
      _clear_MCUCR_JTD_bit();  // Disables JTAG interface
      Serial1.begin(9600);
      /* Jaap: Intended for use with usb connection.
       * Prevents app from being able to connect.
       * Temporarily commented out. Remove?
       */
      //while (!Serial) {}       // wait for USB serial port to connect. Needed for Leonardo only
#endif
      pinMode(PWMA, OUTPUT);
      pinMode(PWMB, OUTPUT);
      analogWrite(PWMA, 0);
      analogWrite(PWMB, 0);

      pinMode(LED, OUTPUT);        // initialize LED indicator as an output.

      telecom.begin();
}

void ArxRobot::loop()
{
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  // note: Leonardo does not support serialEvent() handler
    //if(Serial1.available())commandProcessor();
    if(Serial1.available()){
        commandProcessor();
    }
#else
    if(Serial.available())commandProcessor();
#endif
    telecom.sendData();
}

/*
 * assign list of commands defined by the user
 */
void ArxRobot::setOnCommand(cmdFunc_t* functions,uint8_t arraysize)
{
  _onCommand = functions;
  _arraysize = arraysize;
}

/*********************
 *  Private Methods  *
 *********************/

void ArxRobot::commandProcessor()
{
    uint8_t cmd = telecom.commandDecoder();
    if (cmd != 0){
        // reset watchdog timer
        wdt_reset();    // included in avr/wdt.h library, assembly instruction wdr

        // determine nature of command (internal or user defined)
        // has user defined any commands?
        if (_arraysize > 0){
            // yes, is this a user defined command?
            uint8_t index = search_onCommand(cmd);
            if (index != 0xFF){
                // yes, call user defined command handler
                uint8_t  n = telecom.getLength();     // number of arguments
                uint8_t *d = telecom.getData();       // points to arguments in data array
                _onCommand[index].funct (cmd, d, n);  // callback
            }
            else{
                // no, call internal command handler
                telecom.commandHandler();
            }
        }
        else{
            // no user defined commands, call internal command handler
            telecom.commandHandler();
        }
    }
}

uint8_t ArxRobot::search_onCommand(uint8_t command_received)
{
  uint8_t found_here = 0xFF;          // returns 0xFF if not found
  for (uint8_t i=0; i < _arraysize; i++)
  {
      if (command_received == _onCommand[i].cmd) {
          found_here = i;
          break;
      }
  }
  return found_here;
}

/* Set JTD bit in MCUCR Register to False
 * Disables JTAG Interface and Allows Access To Port F Pins 7 - 4
 */
void ArxRobot::_clear_MCUCR_JTD_bit()
{
  asm("in r24, 0x35");
  asm("ori r24, 0x80");
  asm("out 0x35, r24");
  asm("out 0x35, r24");
}

/* Notes
1. The callback construction is modeled on the MIDI library .h file and
   MIDI_Callbacks.ino example.

2. Lines limit is 80 characters as defined by Google C++ Style Guide
0         1          2        3         4         5         6         7        7
01234567890123456789012345678901234567890123456789012345678901234567890123456789

3. How to tell compiler that name will be defined before linker is run.
extern const uint8_t CMD_LIST_SIZE;

*/
