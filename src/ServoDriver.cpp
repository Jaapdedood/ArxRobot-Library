/*
  Created by Jaap de Dood, July 24, 2019
*/

#include "Arduino.h"
#include "Configure.h"
#include "ServoDriver.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor

ServoDriver::ServoDriver()
{
}

/*
 * Initializes each servo and it's min/max ranges
 * Note: number of servos passed as argument since cannot be
 * obtained within method (treated as pointers inside functions).
 *
 * Do not forget to obtain and validate array lengths before calling
 * this method.
 */
void ServoDriver::begin(uint8_t* pins, uint8_t* maxes, uint8_t* mins, uint8_t servoNos)
{
    for(int i = 0;i < servoNos; i++)
    {
        myServos[i].attach(pins[i]);
    }
}

/*
 * Set servo angles corresponding to data received by command
 * TODO: This function could be done without servoNos, possibly less safe? Decide. Same for reset and home.
 */
void ServoDriver::servos_go(uint8_t* servodata, uint8_t servoNos)
{
    uint8_t angles[servoNos];

    for(int i = 0; i < servoNos; i++)
    {
        angles[i] = (servodata[3+i] << 8) | servodata[4+i];
    }

    cli();
    for(int i = 0; i < servoNos; i++)
    {
        myServos[i].write(angles[i]);
    }
    sei();
}


/*
 * Set the servos in a reset angle
 */
void ServoDriver::servos_reset(uint8_t servoNos)
{
    for(int i = 0; i < servoNos; i++)
    {
        myServos[i].write(90);
    }
}

/*
 * Set the servos in the home defined angle
 */
void ServoDriver::servos_home(uint8_t* commandData, uint8_t servoNos)
{
    for(int i = 0; i < servoNos; i++)
    {
        myServos[i].write(commandData[3]);
    }
}
