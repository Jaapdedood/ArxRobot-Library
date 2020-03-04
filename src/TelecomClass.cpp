/*
  TelecomClass.cpp - Library for handling telecommunications
  Created by Gary Hill, August 13, 2016.
  Edited by Jaap de Dood, Jeff Gomes
*/

#include "Arduino.h"       // Arduino library files
#include <EEPROM.h>
#include "Configure.h"       // 3DoT Hardware Abstraction Layer

#include "FuelGauge.h"     // Fuel Gauge sensor
#include "Packet.h"        // packetize and send data to the 3DoT application => Arxterra control panel
#include "MotorDriver.h"     // DRV8848 Motor Driver
#include "Watchdog.h"
#include "TelecomClass.h"

// instantiate objectscommandDecoder
Packet batteryPacket(BATTERY_ID); // battery telemetry
Packet telecomPacket(0x00);       // id set by context
DRV8848 motor_driver;
FuelGauge batteryGauge(BATTERY_ID, VBATT_PIN);  // default LiPO
Watchdog watchdogTimer;

/*
 *  C++ .cpp member class definitions
 */

/*
#include "Arduino.h"
#include "Packet.h"
*/

// Constructor
TelecomClass::TelecomClass()
{
}

/*
 * Initialize TelecomClass
 */
void TelecomClass::begin()             // initialize the packet
{
  // initialize objects
  motor_driver.begin();
  batteryGauge.begin();                // default LiPO
  batteryPacket.setAccuracy(4);        // change sensor accuracy to +/-4 DN
  batteryPacket.setSamplePeriod(5000); // change sample period from 5 seconds

  // initialize variables
  _command = 0;                        // optional
  _length = 0;
}

/*
 * Getters
 */


uint8_t* TelecomClass::getData(){
  return _data+3;         // arguments start at index 3
}

// N-1:
uint8_t TelecomClass::getLength(){
  return _length-1;       // number of arguments is packet length N minus command byte
}

/************************  Send Data ************************
 *   This is where sensor value is checked and if different  *
 *  a data packet of three bytes (Command ID and associated) *
 *                is sent from Arduino to Phone              *
 *************************************************************/
void TelecomClass::sendData()
{
  uint16_t percentage = batteryGauge.readFuelGauge();
  batteryPacket.sendSensor(percentage);       // send a 16-bit unsigned word to the control panel.
  if (percentage == 0) {                      // battery has been depleted
      motor_driver.motors_safe();               // Need to actually sleep the rover
#if DEBUG                                 // Message repeats until unit is turned off and battery charged
      Serial.print("Battery Undervoltage = ");
      Serial.println(batteryGauge.getVoltage()); // data type is float
    #endif
  }
}

/******************************  Command Decoder ******************************
 *   This is where a command is received and decoded. FSM implemented with     *
 *   Mealy & Moore output decoder section.                                     *
 *                                                                             *
 *   Output:                                                                   *
 *     0         | Processing command or processing complete with error        *
 *     otherwise | Command                                                     *
 *                                                                             *
 *   Operational Note: The Serial input buffer typically holds 1 byte before   *
 *   the Serial.read operation. Consequently, the While loop may only run once *
 *   per byte. Therefore calling should check return value.                    *
 ******************************************************************************/

uint8_t TelecomClass::commandDecoder()
{
  static uint8_t state = 0;    // initially in state
  static uint8_t i = 0;        // index pointer for data array
  static uint8_t N = 0;        // bytes in the Command Data Packet Field
  static uint8_t checksum = 0; // LRC byte
  uint8_t next_state;          // next state

  _command = 0;                // processing command

  // Step through FSM until packet received.
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  while(Serial1.available()) // note: Leonardo does not support serialEvent() handler
  {
    _data[i] = Serial1.read();
  #else
  while(Serial.available())
  {
    _data[i] = Serial.read();
  #endif

    // FSM implementation
    // Mealy and Moore output decoder section
    checksum ^= _data[i];      // includes even parity byte

    // Next State Decoder with Mealy output decoder section
    switch (state){  // set dir = motordata[1] for motor A (left)
    case 0:   // Start Byte
        if (_data[0] != COMMAND_PACKET_ID){      // start code 0xA5, test code 0x73 = 's'
            throwError(word(0x01,_data[0]));       // word() casts as uint16_t on Uno class MCUs and 32-bits on Zero class MCUs
            i = 0;
            checksum = 0;
            next_state = 4;  // exception
        }
        else{
          i = 1;           // or i++;
          next_state = 1;  // read packet length
        }
        break;

      case 1:  // Packet Length
        N = _data[1];
        if (0 < N && N <=20){
          i = 2;          // or i++;
          next_state = 2;
        }
        else{
          throwError(word(0x02,N));
          i = 0;
          checksum = 0;
          next_state = 4;  // exception
        }
        break;

      case 2:  // Read Command Data Packet Field
        i++;
        if (i < N+2) next_state = 2;
        else next_state = 3;
        break;

      case 3:  // Checksum
        if (checksum == 0){
          // set properties
          _command = _data[2];      // command decoded
          _length = N;
          // reset local variables
          i = 0;
          checksum = 0;
          next_state = 0;
          #if ECHO_COMMANDS
          commandEcho(_data, N);
          digitalWrite(LED, _command & 0x01);   // turn LED ON/OFF based on command qualifier bit ****
          #endif
        }
        else{
          throwError(word(0x03,checksum));
          i = 0;
          checksum = 0;
          next_state = 4;  // exception
        }
        break;

      case 4:  // Exception Handler
        // destroy data until start byte received
        if (_data[0] != COMMAND_PACKET_ID){      // start code 0xA5, test code 0x73 = 's'
          i = 0;
          checksum = 0;
          next_state = 4;
        }
        else{
          i = 1;             // or i++
          next_state = 1;
        }
        break;

      default:
        throwError(word(0x04,_data[i-1]));
        i = 0;
        checksum = 0;
        next_state = 4;  // exception
      } // end switch-case

    /* Array out of bounds check
     * 1. The packet length test N precludes this error from ever occuring (at least in theory)
     * 2. Within this serial1.read while loop, when the array index equals 22
     *    the FSM must be in state 3 (checksum).
     */
    if (i > 22){
      throwError(word(0x05,_data[22]));
      checksum = 0;
      i = 0;
      next_state = 4;           // FSM exception
    }

    // clock FSM
    state = next_state;
  } // end while
  return _command;
} // end commandDecoder

 /******************** Command Handler **********************
 * Implement internal Command ID                             *
 *************************************************************/
void TelecomClass::commandHandler()
{
  // EEPROM
  uint16_t eeprom_address;
  uint8_t  eeprom_data[16];        // 16 bytes maximum

  /*
   * motion command = 0x01
   * motordata[3]   left run    (FORWARD = index 1, BACKWARD = index 2, BRAKE = index 3, RELEASE = index 4)
   * motordata[4]   left speed  0 - 255
   * motordata[5]   right run   (FORWARD, BACKWARD, BRAKE, RELEASE)
   * motordata[6]   right speed 0 - 255
   * example
   * forward half speed  0x01, 0x01, 0x80, 0x01, 0x80 0101800180
   */
  if (_command == MOVE)
  {
    motor_driver.motors_go(_data);       // Sparkfun DRV8848 Motor Driver
  }

  /*
   * EEPROM
   * Telemetry Table = Robot Capabilities Worksheet
   * data[3]    Address High
   * data[4]    Address Low
   * data[5]    Number Bytes
   * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
   * N =        1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
   */
  else if (_command == READ_EEPROM)
  {
    eeprom_address = word(_data[3],_data[4]);
    for (int i=0; i<_data[5]; i++){
      eeprom_data[i] = EEPROM.read(eeprom_address);
      eeprom_address++;
    }

    telecomPacket.sendPacket(EEPROM_RESPONSE_ID, eeprom_data, _data[5]);  // send byte array
  }

  /*
   * EEPROM                                   Test
   * data[3]    Address High                   00
   * data[4]    Address Low                    00
   * data[5]    Number Bytes                   10
   * data[6]    Data (Most Significant Byte)   see test data below
   * data[7]    ...
   * data[N+1]    Data (Least Significant Byte) N
   * i =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
   * N =        1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
   *                        |
   * test data              1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
   */
  else if (_command == WRITE_EEPROM)
  {
    eeprom_address = word(_data[3],_data[4]);
    for (int i=6; i<_data[5]+6; i++){
      EEPROM.write(eeprom_address, _data[i]);
      eeprom_address++;
      delay(4);  // An EEPROM write takes 3.3 ms to complete. (optional)
    }
  }

  /*
   * safe rover
   * motors to idle
   */
  else if (_command == SAFE_ROVER)
  {
    motor_driver.motors_safe();
  }

  /*
   * Reset Watchdog Mode with Echo
   * note: all received commands reset wdt
   */
  else if (_command == PING)
  {
      telecomPacket.sendPacket(PONG_ID);                  // send 8-bit packet ID only
  }
  /*
   * Set Watchdog Mode and Timeout period
   * data[3]    wdt mode and prescaler
   */
  else if (_command == WATCHDOG_SETUP){
    watchdogTimer.watchdogSetup(_data[3]);                // set wdt mode and prescaler

    #if DEBUG
    Serial.print("wdt prescaler set to: ");
    Serial.println(_data[3], HEX);
    #endif
  }
}

/*
 * Private Method Definition(s)
 */

void TelecomClass::throwError(uint16_t err){
  _command = 0;                                  // processing complete with error
  telecomPacket.sendPacket(EXCEPTION_ID,err);    // send 0x0E plus 16-bit FSM error code
  #if DEBUG
  Serial.print("Command decoder exception 0x0"); // Send duplicate data as text to
  Serial.println(err,HEX);                       // USB=>Arduino IDE Serial Monitor.
  #endif
}

// **** TODO convert to properties
void TelecomClass::commandEcho(uint8_t * data, uint8_t N)
{
                              // i = 0     1     2  ...    N + 2
  uint8_t data_out[N];        // N =             1 .. N
  for(int i=2; i<N+2; i++){   //    A5, Length,          , Checksum;
    data_out[i-2] = data[i];  //               [ID, Data]
  }
  telecomPacket.sendPacket(COMMAND_ECHO_ID, data_out, N);  // send byte array packet over USB or bluetooth

  #if DEBUG
  Serial.print("rover received command: ");  // Send duplicate data as text to
  Serial.print(data[2], HEX);                // USB=>Arduino IDE Serial Monitor.
  for(int i=3; i<N+2; i++){
  Serial.print(", ");
  Serial.print(data[i], HEX);
  }
  Serial.println();
  #endif
}
