/******************* 3DoT Configure ****************
   source: 3DoT_v8.01 schematic
***************************************************/
#ifndef Configure_h
#define Configure_h

/******************* Debug Mode ********************
 * When debugging your program set DEBUG and ECHO_COMMANDS to TRUE. Once your code is bug free,
 * to avoid latency errors and minimize bandwidth set to FALSE (the default value).
 ***************************************************/
#define FALSE  0
#define TRUE   1
#define DEBUG  FALSE
#define ECHO_COMMANDS  TRUE

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
//Code in here will only be compiled if an Arduino Leonardo is used.
const int LED = 14;  // RXLED LED on 3DoT board, TXLED wired to PD5
#else
const int LED = 13;  // Arduino UNO and Rosco wired to 13
#endif

const int STBY = 8;  // standby  (Sparkfun ProMicro Analog Pin A0)

//Motor A
const int PWMA = 6;  // Speed control
const int AIN1 = 12;  // Direction
const int AIN2 = 4; // Direction

//Motor B
const int PWMB = 10;  // Speed control
const int BIN1 = 9; // Direction (Analog Pin A1)
const int BIN2 = 5; // Direction (Analog Pin A2)

const int VBATT_PIN = A5; // A5 is an Arduino reserved word
const int SERVO_11 = 7;
const int SERVO_13 = 11;
const int I2C_SDA = 2;    // 3.3v I2C
const int I2C_SDL = 3;

/******************* Packet IDs ********************
   source: Arxterra
 ***************************************************/
#define COMMAND_PACKET_ID    0xA5
#define TELEMETRY_PACKET_ID  0xCA

/******************* Robot Commands ****************
   source: Arxterra Technical Folder / Communication between Arduino
           Phone, and Control Panel - Phone to Arduino (starting at line 57)
 ***************************************************/
// Arxterra Commands to Numeric Value Mapping
//               Data[0] =           CMD TYPE | Qual | Arguments
//                                     7-bits  1-bit   Bytes (note: N = 1 + bytes)
//                              bit => 7654321   0
#define MOVE                 0x01   // 0000000   1     4
#define CAMERA_MOVE          0x02   // 0000001   0     4
#define CAMERA_MOVE_HOME     0x04   // 0000010   0     0
#define CAMERA_MOVE_RESET    0x05   // 0000010   1     0
#define READ_EEPROM          0x06   // 0000011   0     3
#define WRITE_EEPROM         0x07   // 0000011   1     3 + b
#define SAFE_ROVER           0x08   // 0000100   0     0
#define SLEEP                0x0A   // 0000101   0     0
#define WAKEUP               0x0B   // 0000101   1     0
#define HEADLIGHT_OFF        0x0C   // 0000110   0     0
#define HEADLIGHT_ON         0x0D   // 0000110   1     0
#define WATCHDOG_SETUP       0x10   // 0001000   0     1   => see table following (pre-implementation 7/28/2016 PING_INTERVAL allowed for 4 byte argument)
#define PING                 0x11   // 0001000   1     0
#define HEADING              0x12   // 0001001   0     2
#define CURRENT_COORD        0x13   // 0001001   1     see note [1]
#define WAYPOINT_COORD       0x14   // 0001010   0     see note [1]
#define WAYPOINTS_OFF        0x16   // 0001011   0     0
#define WAYPOINTS_ON         0x17   // 0001011   1     0
#define WAYPOINTS_CLEAR_ALL  0x18   // 0001100   0     0
#define WAYPOINT_MOVE        0x19   // 0001100   1     9
#define WAYPOINT_DELETE      0x1A   // 0001101   0     1
#define WAYPOINT_VIEW_CLICK  0x1B   // 0001101   1     4

// note(s)
// [1] See Waypoint Coordinates class in "Communication between Arduino,
//     Phone, and Control Panel" Google Drive Spreadsheet.

// 0x40 - 0x5F  32 Custom Commands, 1 - 2 byte arguments

/********************** 0x10 WATCHDOG SETUP *****************
   byte
   0x00  Watchdog Off
   0x4E  1 sec interrupt and system reset mode
   0x4F  2 sec
   0x68  4 sec
   0x69  8 sec
   0x46  1 sec interrupt mode
   0x47  2 sec
   0x60  4 sec
   0x61  8 sec
*************************************************************/

/******************* Robot Telemetry ****************
   source: Arxterra Technical Folder / Communication between Arduino
           Phone, and Control Panel - Arduino to Phone (starting at line 84)
           see reference document for argument (typically assume int (i.e., 16 bit signed)
 ***************************************************/
// Telemetry Identifiers to Numeric Value Mapping
const uint8_t MOTOR1_CURRENT_ID  = 0x01;  // motor 1 is left motor
const uint8_t MOTOR2_CURRENT_ID  = 0x02;  // motor 2 is right motor
const uint8_t TEMP_SENSOR_ID     = 0x03;  // temperature sensor
const uint8_t RANGE_LEFT_ID      = 0x04;  // ultrasonic range 1 is left
const uint8_t RANGE_RIGHT_ID     = 0x05;  // ultrasonic range 2 is right
const uint8_t BATTERY_ID         = 0x06;  // Arduino battery
const uint8_t DIRTY_BATTERY_ID   = 0x07;  // DC and servo motors battery
const uint8_t PAN_POSITION_ID    = 0x08;  // originally defined as pan and tilt
const uint8_t TILT_POSITION_ID   = 0x09;  // not in original definition
const uint8_t EEPROM_RESPONSE_ID = 0x0A;  // sent in response to EEPROM Read Command
const uint8_t EMERGENCY_ID       = 0x0B;  // arduino emergency code (see subtable below)

const uint8_t COMMAND_ECHO_ID    = 0x0D;
const uint8_t EXCEPTION_ID       = 0x0E;  // arduino error code exception (see subtable below)
const uint8_t PONG_ID            = 0x11;
const uint8_t ROUTE_STATUS_ID    = 0x12;
const uint8_t WAYPOINT_ARRIVE_ID = 0x13;

/********************** 0x0E Exception Codes *****************
   high   low order byte
    01    Start byte 0xA5 expected
    02    Packet length out of range 1 - 20
    03    LRC checksum error
    04    Undefined command decoder FSM state
    05    Array out of range i >= 23
    06    Watchdog timeout out of range
*************************************************************/

/********************** 0x0B Emergency Codes *****************
    high   low order byte
    01     00               Watchdog timeout
*************************************************************/
const uint16_t WATCHDOG_TIMEOUT = 0x0100;

#endif
