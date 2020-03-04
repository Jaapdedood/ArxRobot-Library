/*
 * ArxRobot Sample Arduino Code
 * Basic Example
 */

#include <ArxRobot.h>
#include <Wire.h>                   // ready for Dot shields

ArxRobot ArxRobot;                  // make a 3DoT Robot

void setup()
{
    Serial.begin(9600);             // default = 115200
    ArxRobot.begin();               // initialize hardware
    ArxRobot.setCurrentLimit(0);   // set current limit to max current ~= 500mA per motor
                                    // to set a lower current limit, see https://www.arxterra.com/current-limit/
}

void loop()
{
    ArxRobot.loop();                // handle ArxRobot commands
}
