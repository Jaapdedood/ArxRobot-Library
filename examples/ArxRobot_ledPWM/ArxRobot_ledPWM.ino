#include <ArxRobot.h>
#define LED 3

ArxRobot ArxRobot; // Instantiate ArxRobot

void ledPWM (uint8_t cmd, uint8_t param[], uint8_t n)
{
  // Changes the brightness of an LED by setting teh PWM value to slider value
  analogWrite(LED, param[0]);
}

void setup()
{
  ArxRobot.begin();

  ArxRobot.addCustomCommand(myCommand, 0);

  pinMode(LED, OUTPUT);
}

void loop()
{
  ArxRobot.loop();
}
