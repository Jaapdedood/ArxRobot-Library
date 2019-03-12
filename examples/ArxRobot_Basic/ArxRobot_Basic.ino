/*
 * ArxRobot Sample Arduino Code
 * Basic Example
 */

#include <ArxRobot.h>
#include <EEPROM.h>
#include <Wire.h>               // ready for Dot shields

ArxRobot ArxRobot;       // make a 3DoT Robot

void setup()
{
    Serial.begin(9600);           // default = 115200
    ArxRobot.begin();
}

void loop()
{
    ArxRobot.loop();
}
