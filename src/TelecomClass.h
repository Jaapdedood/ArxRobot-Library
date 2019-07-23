/*
  TelecomClass.h - Library for handling telecommunications
  Created by Gary Hill, August 13, 2016.
  Modified by Jaap de Dood.
*/

#ifndef TelecomClass_h     // The #define Guard
#define TelecomClass_h

#include "Arduino.h"       // Arduino library files
#include <EEPROM.h>
#include "Configure.h"     // 3DoT Hardware Abstraction Layer

#include "PowerManagement.h"     // Fuel Gauge sensor and Current limiting
#include "Packet.h"        // packetize and send data to the 3DoT application => Arxterra control panel
#include "TB6612FNG.h"     // TB6612FNG Motor Driver
#include "Watchdog.h"
#include "TwoServoDriver.h"

/*
 *  header .h class member definitions
 */

class TelecomClass
{
  public:

  // Constructor
  TelecomClass();

  // Public class 'methods'
  void begin();
  void sendData();
  uint8_t commandDecoder();
  void commandHandler();

  // Getters
  uint8_t* getData();
  uint8_t getLength();

  private:
  // Private class 'methods'
  void throwError(uint16_t);
  void commandEcho(uint8_t *, uint8_t);

  // Private class 'properties'
  uint8_t  _command;   // 0 = processing or processing complete with error, otherwise command
  uint8_t  _data[23];  // 23 bytes = ID + Length + CMD + maximum number of parameters 19 + Checksum
                       // EEPROM read/write maximum block size = 16
                       // maximum index 22
  uint8_t  _length;

};   // end of class definition

#endif

 /* Test Sequences
 * i =  0  1  2  3  4  5  6  7
 * N =        1  2  3  4  5
 *     A5 05 01 01 80 02 80 A2   MOV turn right forward-backward half speed
 *
 * i =  0  1  2  3
 * N =        1
 *     A5 01 04 A0  Camera Home
 *     A5 01 05 A1  Camera Reset
 *     A5 01 08 AC  Safe Rover
 *     A5 01 0A AE  Sleep
 *     A5 01 0B AF  Wake
 *
 * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 * N =        1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
 *     A5 14 07 00 00 10 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 B6
 * Write a 16 byte block to EEPROM
 * Command Packet ID A5
 * N = 20 or 0x14
 * EEPROM Write Command  07
 * EEPROM Address 0x0000 00 00
 * Number of Bytes 16    10
 * Data  01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 * Parity Byte B6
 *
 * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 * N =        1  2  3  4
 *     A5 04 06 00 00 10 B7
 * Read a 16 byte block from EEPROM starting at address 0x0000
 * Command Packet ID A5
 * Data field length N = 4
 * EEPROM Read Command 06
 * EEPROM Address 0x0000 00 00
 * Number of Bytes 16    10
 * Parity Byte B7

 * EEPROM Response Example
 * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 * N =        1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
 *     CA 11 0A 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 *                                                               | <- checksum
 * Telemetry Packet ID   CA
 * N = 17 or 0x11 response does not include address or number of bytes,
 *                which was included in the request.
 * EEPROM Read Response  0A
 * Data  01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 - assuming this data was written to EEPROM
 * Parity Byte C1
 */

 /************ FSM Generated Exception Codes ****************
 *   01    Start byte 0xA5 expected
 *   02    Packet length out of range 1 - 20
 *   03    LRC checksum error
 *   04    Undefined command decoder FSM state
 *   05    Array out of range i >= 23
 ************************************************************/

/******************** Command Acquisitions *******************
 * When using Bluetooth or USB OTG (On The Go) this is where
 * the Command ID and associated bytes are obtained.
 * Packet Structure based on CCSDS Standards
 * http://public.ccsds.org/publications/archive/102x0b5s.pdf *
 *
 * State Byte
 *       Index  Bits  Description        Default            MOV example
 *   0   0       3    3.1.1 Version Number 101
 *               3    3.1.2 Packet ID         001
 *               2    Reserved                   01         data[0] = 0xA5
 *   1   1       4    Reserved             000
 *               4    3.1.4 Packet Length     00001 - 10100 data[1] = 0x05 Bytes in Command Data Field
 *                                                                         excluding checksum
 *   2   2            3.2 Command Data Packet Field         data[2] = 0x01 MOV command
 *       :                                                       :
 *       N+1          where N = 00001 - 10100               data[6] = 4th parameter
 *   3   N+2          checksum                              data[7] = longitudinal redundancy check (LRC)
 *                    warning: does not protect against byte reversals http://en.wikipedia.org/wiki/Checksum
 *   4                Error State
 ************************************************************/

 /*************************************************************
 * You can read more about the CCSDS standards at the
 * following links.
 * http://public.ccsds.org/default.aspx
 * Packet Structure based on CCSDS Standards
 * http://public.ccsds.org/publications/archive/735x1b1.pdf
 * telecommand
 * http://public.ccsds.org/publications/archive/202x1b2s.pdf
 * http://public.ccsds.org/publications/archive/200x0g6.pdf
 * telemetry packet
 * http://public.ccsds.org/publications/archive/102x0b5s.pdf
 *************************************************************/

 /*************************************************************
 * Convert 4 data bytes in array to 32 bit unsigned long
 * uint32_t pingInterval = (((unsigned long)_data[3]) << 24) | (((unsigned long)_data[4]) << 16) | (((unsigned long)_data[5]) << 8) | (unsigned long)_data[6];
 *************************************************************/
