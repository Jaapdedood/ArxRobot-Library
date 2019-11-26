/*
  Packet.cpp - Library for qualifying, packetizing, and transmitting sensor data.
  Data is qualified by bits needing to change for a new packet to be generated
  (accuracy property) and minimum time between packets (samplePeriod property).
  Created by Gary Hill, August 13, 2016.

  Discussion of overflow included at the bottom of the .cpp page
*/

#include "Arduino.h"       // Arduino library files
#include <limits.h>      // Defines constants with the limits of fundamental integral types (UINT_MAX = 2^16 - 1)
#include "Configure.h"     // 3DoT Hardware Abstraction Layer

#include "FuelGauge.h"     // Fuel Gauge sensor
#include "Packet.h"        // packetize and send data to the 3DoT application => Arxterra control panel
#include "TB6612FNG.h"     // TB6612FNG Motor Driver
#include "Watchdog.h"

/*
 *  C++ .cpp member class definitions
 */

// Constructor
Packet::Packet(uint8_t id)
{
    _id = id;                // telemetry identifier
    _nextTime = 0;           // immediatly send first packet
    _accuracy = 2;           // default accuracy of measurement is +/- 2 DN
    _samplePeriod = 1000;    // default sample period is 1 second (f = 1 Hz)
    _lower_limit = UINT_MAX; // intial test will fail one of these 2 conditions
    _upper_limit = 0x0000;
}

// Use this version of sendPacket to transmit the ID byte, without any arguments
// sendID
void Packet::sendPacket(uint8_t param_id)
{
    uint8_t data[4];  // 4 bytes = ID + Length + id + Checksum
    data[0] = TELEMETRY_PACKET_ID;
    data[1] = 1;             // id only
    data[2] = param_id;      // do not update private id
    data[3] = data[0] ^ data[1] ^ data[2];  // update LRC

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    Serial1.write(data,4); // default, write to USART => Bluetooth
#else
    Serial.write(data,4);  // N+4 = number of bytes to send.  write to USB.
#endif
}

// sendWordPacket
void Packet::sendPacket(uint8_t param_id, uint16_t value)
{
    uint8_t wordData [2] = {highByte(value), lowByte(value)};
    sendPacket(param_id, wordData, 2);
}

// sendPacket transmits a byte array
// sendByteArrayPacket
void  Packet::sendPacket(uint8_t param_id, uint8_t data_field[], uint8_t N)  // alt. uint8_t * data_field
{
    static uint8_t data[23];  // 23 bytes = ID + Length + SENSOR + maximum number of parameters 19 + Checksum
                              // EEPROM read/write maximum block size = 16
                              // maximum index 22
    uint8_t datum;
    uint8_t checksum = 0;     // LRC byte

    data[0] = 0xCA;           // TELEMETRY_PACKET_ID (or #include "Configure.h")
    data[1] = N+1;            // id + eeprom data
    data[2] = param_id;
    checksum = data[0] ^ data[1] ^ data[2];  // update LRC
    for(uint8_t i = 0; i < N; i++){
        datum = data_field[i];
        data[i+3] = datum;
        checksum ^= datum;
    }
    data[N+3] = checksum;      // N+3 = last byte address

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    Serial1.write(data,N+4);          // default, write to USART => Bluetooth
#else
    Serial.write(data,N+4);  // N+4 = number of bytes to send.  write to USB.
#endif
}

bool Packet::sendSensor(uint16_t value)
{
    bool change_in_value = !((_lower_limit <= value) && (value <= _upper_limit));
    bool sample_period = millis() > _nextTime;
    if (change_in_value && sample_period) {
        uint8_t wordData [2] = {highByte(value), lowByte(value)};
        sendPacket(_id, wordData, 2);
        // update properties
        _lower_limit = value - _accuracy;
        // check for overflow (see discussion at the end of this page)
        if (_lower_limit > value) _lower_limit = 0x0000;  // Alternatively "lower_limit > _accuracy" should also work

        _upper_limit = value + _accuracy;
        // check for overflow (see discussion at the end of this page)
        if (_upper_limit < value) _upper_limit = UINT_MAX;

        _nextTime = millis() + _samplePeriod;

        return true;
    }
    return false;
}

void  Packet::setSamplePeriod(uint32_t samplePeriod)
{
    _samplePeriod = samplePeriod;
}

void  Packet::setAccuracy(uint16_t accuracy)
{
    _accuracy = accuracy;
}

/*
 * Private Method Definition(s)
 */
