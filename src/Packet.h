/*
  Packet.h - Library for qualifying, packetizing, and transmitting sensor data.
  Data is qualified by bits needing to change for a new packet to be generated
  (accuracy property) and minimum time between packets (samplePeriod property).
  Created by Gary Hill, August 13, 2016.

  Discussion of overflow included at the bottom of the page
*/
#ifndef Packet_h         // The #define Guard
#define Packet_h

#include "Arduino.h"     // Arduino library files
#include <limits.h>      // Defines constants with the limits of fundamental integral types (UINT_MAX = 2^16 - 1)

/*
 *  header .h class member definitions
 */

class Packet
{
public:

    // Constructor
    Packet(uint8_t id);

    // Public class 'methods'
    // overloaded method
    void sendPacket();                             // send telemetry type id property _id
    void sendPacket(uint8_t);                      // send telemetry type id only
    void sendPacket(uint8_t, uint16_t);            // send telemetry type id with unsigned 16-bit word
    void sendPacket(uint8_t, uint8_t *, uint8_t);  // send telemetry type id with byte array

    bool sendSensor(uint16_t);                     // send qualified unsigned 16-bit word

    void setSamplePeriod(uint32_t);
    void setAccuracy(uint16_t);

private:

    // Private class 'methods'

    // Private class 'properties'
    uint8_t  _id;
    uint32_t _nextTime;        // underscore denotes private property
    uint16_t _accuracy;
    uint32_t _samplePeriod;
    uint16_t _lower_limit;
    uint16_t _upper_limit;

};   // end of class definition

#endif

/******************* Emergency Flags ****************
 * source: Found in CommunicationRobotPilot Folder
 ***************************************************/
/*

all clear = 0 or null,
set = One Byte combinable flags bits{
 1 = latency           0b00000001
 2 = obstacle,         0b00000010
 4 = stall,            0b00000100
 8 = unsafe position,  0b00001000
 16 = battery,         0b00010000
 32 = temperature      0b00100000
 64 = command time-out ==> from arduino
*/

/********************** Telemetry Packet *********************
 * When using Bluetooth or OTG this is where the Command ID  *
 * and associated bytes are obtained.
 * Packet Structure based on CCSDS Standards
 * http://public.ccsds.org/publications/archive/102x0b5s.pdf *
 *
 * State Byte
 *       Index  Bits  Description        Default            EEPROM Example
 *   0   0       3    3.1.1 Version Number 110
 *               3    3.1.2 Packet ID         010
 *               2    Reserved                   10         data[0] = 0xCA
 *   1   1       4    Reserved             000
 *               4    3.1.4 Packet Length     00001 - 10100 data[1] = 0x14 Bytes in Telemetry Data Field
 *                                                                         excluding checksum
 *   2   2            3.2 Telemetry Data Packet Field       data[2] = 0x0A EEPROM response
 *       :                                                       :
 *       N+1          where N = 00001 - 10100               data[21] = 16th parameter
 *   3   N+2          checksum                              data[22] = longitudinal redundancy check (LRC)
 *                    warning: does not protect against byte reversals http://en.wikipedia.org/wiki/Checksum
 *   4                Error State
 ************************************************************/

/* EEPROM Response Example
 * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 * test data     1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 *                                                               | <- checksum
 * note: response does not include address or number of bytes,
 *       which was included in the request.
 */

/* sendPacket is an example of function overloading (or compile time polymorphism)
 * Overloading is when you have functions with the same name but different parameters.
 * The parameters may differ in their type or number, or in both.
 * When the C++ compliler encounters a call to an overloaded method, it sets the
 * destination of the call to the version of the method whose parameters match the
 * arguments used in the call.
 */

/*  Test for overflow
 *  These are a number of ways to test for overflow as covered in this discussion.
 *  http://stackoverflow.com/questions/199333/how-to-detect-integer-overflow-in-c-c
 *  1. Test SREG OV flag bit directly. May need to define as volatile http://blog.regehr.org/archives/28.
 *  2. Test if answer is greater than any of the numbers.
 *  3. Cast to more bits (32), perform operation, and test with limit UINT_MAX (2^16-1), cast back original size (16).
 */
