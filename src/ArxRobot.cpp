/*
  ArxRobot.cpp - ArxRobot Base Class
*/

#include "Arduino.h"       // Arduino library files
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

/* create an instance of a packet to send the value from the 
 * Output Compare Register Timer 4D wired to PWM signal to Motor B 
 */
TelecomClass telecom;

/*
 *  C++ .cpp member class definitions
 */

// Constructor
ArxRobot::ArxRobot()
{

}

/*********************
 *  Public Methods   *
 *********************/

/*
 * initialize ArxRobot properties to default values
 */
void ArxRobot::begin()
{
    wdt_disable();    // the watchdog timer remains active even after a system reset
                      // (except a power-on condition) hopefully Arduino bootloader does this,
                      // because by now it is probably to late to prevent
                      // an eternal loop of time-out resets.

    _clear_MCUCR_JTD_bit();  // Disables JTAG interface
    Serial1.begin(9600);

    pinMode(LED, OUTPUT);        // initialize LED indicator as an output.

    telecom.begin();
}

void ArxRobot::loop()
{
    if(Serial1.available())
    {
        commandProcessor();
    }

    telecom.sendData();
}

/*
 * assign list of commands defined by the user
 */

void ArxRobot::addCustomCommand(fptr_t function, uint8_t commandID)
{
    _customCommands[commandID].funct = function;
    _customCommands[commandID].cmd = commandID;
}

void ArxRobot::replaceBuiltInCommand(uint8_t commandID, fptr_t function)
{
    _interceptCommands[commandID].funct = function;
    _interceptCommands[commandID].cmd = commandID;
}

uint16_t ArxRobot::readBatteryVoltage()
{
    pinMode(A5, INPUT);
    uint16_t totalVoltage = 0;
    for(int i=0;i<50;i++)
    {
      uint16_t VBATT = analogRead(A5);
      totalVoltage = totalVoltage + VBATT;    
    }
    uint16_t averageVoltage = totalVoltage/5;
    return averageVoltage;
}

void ArxRobot::alertFatalError()
{
    // TODO: something reasonable here
}

/*********************
 *  Private Methods  *
 *********************/

void ArxRobot::commandProcessor()
{
    uint8_t cmd = telecom.commandDecoder();

    // If a command received
    if (cmd != 0)
    {
        // reset watchdog timer
        wdt_reset();    // included in avr/wdt.h library, assembly instruction wdr

        // Is command a built-in command?
        if(cmd < CUSTOM_COMMANDS_START)
        {
            // Does the user want to intercept command?
            if(_interceptCommands[cmd].funct != NULL)
            {
                //yes, run user command
                uint8_t  n = telecom.getLength();     // number of arguments
                uint8_t *d = telecom.getData();       // points to arguments in data array
                _interceptCommands[cmd].funct (cmd, d, n);
            }
            else
            {
                //no, run built in command
                telecom.commandHandler();
            }
            
        }
        else
        {
                // If custom command exists
                if(_customCommands[cmd-CUSTOM_COMMANDS_START].funct != NULL)
                {
                    //run command
                    uint8_t  n = telecom.getLength();     // number of arguments
                    uint8_t *d = telecom.getData();       // points to arguments in data array
                    _customCommands[cmd-64].funct (cmd, d, n);
                }
                else
                {
                    // TODO: Send error to tell app user to add command matching ID
                }
        }
    }
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

void ArxRobot::debugFunction()
{
    Serial.println(_customCommands[0].cmd);
    Serial.println(_customCommands[1].cmd);
    Serial.println(_customCommands[2].cmd);
    Serial.println(_customCommands[0].funct == NULL);
    Serial.println(_customCommands[1].funct == NULL);
    Serial.println(_customCommands[2].funct == NULL);
}