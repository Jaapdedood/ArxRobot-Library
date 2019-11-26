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
    ArxRobot.setCurrentLimit(60);   // set current limit to 560mA
                                    // see https://www.arxterra.com/current-limit/
}

void loop()
{
    ArxRobot.loop();                // handle ArxRobot commands
}
