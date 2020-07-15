#include <ArxRobot.h>

ArxRobot ArxRobot; // Instantiate ArxRobot

void myCommand (uint8_t cmd, uint8_t param[], uint8_t n)
{
    // Do something e.g. when a button is pressed or a slider is moved on the app.
    digitalWrite(LED_BUILTIN, HIGH);
}

void myOtherCommand (uint8_t cmd, uint8_t param[], uint8_t n)
{
    // Do something else
}  

void myCoolCommand(uint8_t cmd, uint8_t param[], uint8_t n)
{
    // Do something even cooler
}

void myMove(uint8_t cmd, uint8_t param[], uint8_t n)
{
    // Using the app tank slider and d-pad will call this function instead
    digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{
  Serial.begin(9600);             // default = 115200
  ArxRobot.begin();

  // addcustomCommand( name_of_function , commandID )
  ArxRobot.addCustomCommand(myCommand, 0);
  ArxRobot.addCustomCommand(myOtherCommand, 1);

  // replace the MOVE command with myMove
  ArxRobot.replaceBuiltInCommand(MOVE, myMove);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  ArxRobot.loop();
}
