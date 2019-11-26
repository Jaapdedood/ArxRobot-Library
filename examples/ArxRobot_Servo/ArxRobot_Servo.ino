/*
   ArxRobot Sample Arduino Code
   Servo and Current limiting Example
*/

/* Ensure Servo.h is NOT included */
#include <ArxRobot.h>

ArxRobot ArxRobot;              // make a 3DoT Robot

int pos = 0;                    // variable to store the servo position

Servo myservo;                  // create servo object to control a servo

void setup()
{
  Serial.begin(9600);           // default = 115200
  myservo.attach(7);            // attaches the servo on pin 7 to the servo object
  
  ArxRobot.begin();
  ArxRobot.setCurrentLimit(60); // set current limit to 560mA
                                // see https://www.arxterra.com/current-limit/
}

void loop()
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
