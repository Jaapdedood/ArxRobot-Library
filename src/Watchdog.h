/*
  Watchdog.h - Library for monitoring the communication link 
  with the application/control panel and the robot.
  Written by Gary Hill, August 13, 2016
*/

#ifndef Watchdog_h       // The #define Guard
#define Watchdog_h

#include "Arduino.h"     // Arduino library files  
#include <avr/wdt.h>     // Standard C library for AVR-GCC avr-libc Arduino\hardware\tools\avr\avr\include\avr\wdt.h
#include "Packet.h"      // packetize and send data to the 3DoT application => Arxterra control panel
#include "MotorDriver.h"   // DRV8848 Motor Driver

/* 
 *  header .h class member definitions
 */

class Watchdog
{
  public: 

  // Constructor
  Watchdog();
  
  // Public class 'methods'
  void watchdogSetup(uint8_t);
  
  private:
  
  // Private class 'methods'
  void watchdogOff();
  void throwError(uint16_t);

  // Private class 'properties'
  uint8_t _prescaler;
  uint8_t _mode;
  uint8_t _counter;
   
};   // end of class definition

#endif

/*
  Code is drawn from these three sources
  1. Watchdog Timer Basic Example
     Written by Nicolas Larsen, 10 June 2011
     http://forum.arduino.cc/index.php?topic=63651.0 

  2. ATmega data sheets
     ATmega48PA/88PA/168PA/328P  Section 10.8 Watchdog Timer (page 50 / 448)
     ATmega16U4/ATmega32U4       Section 8.2  Watchdog Timer (page 48 / 448)

  3. Standard C library for AVR-GCC avr-libc wdt.h library
     Arduino\hardware\tools\avr\avr\include\avr\wdt.h  
     http://www.nongnu.org/avr-libc/user-manual/wdt_8h.html 
*/ 

/*
 * Watchdog Testing
 * Command
 * A5 02 10 00 B7 Watchdog off
 * Interrupt with System Reset
 * A5 02 10 4D FA Set watchdog interrupt for 0.5 sec  error CA 03 0E 06 05 C4, exception error code 06 argument was 05 (0.5 seconds) with LRC = C4
 * A5 02 10 4E 59 Set watchdog interrupt for 1 sec
 * A5 02 10 4F F8 Set watchdog interrupt for 2 sec
 * A5 02 10 68 DF Set watchdog interrupt for 4 sec 
 * A5 02 10 69 DE Set watchdog interrupt for 8 sec
 * A5 02 10 6A DD ATmega reserved
 * Interrupt Mode
 * A5 02 10 45 F2 Set watchdog interrupt for 0.5 sec  error CA 03 0E 06 05 C4, exception error code 06 argument was 05 (0.5 seconds) with LRC = C4
 * A5 02 10 46 F1 Set watchdog interrupt for 1 sec
 * A5 02 10 47 F0 Set watchdog interrupt for 2 sec
 * A5 02 10 60 D7 Set watchdog interrupt for 4 sec 
 * A5 02 10 61 D6 Set watchdog interrupt for 8 sec
 * A5 02 10 62 D5 ATmega reserved,                    error CA 03 0E 06 0A CB
 *                                              exception error = 0E 
 *                                   watchdog timeout out of range = 06 
 *                                                         argument was 0A
 *                                                                   LRC = CB
 * 
 * Telemetry
 * CA 03 0B 01 nn Emergency code = 0B, watchdog timeout = 01, current   prescaler = nn 
 * CA 03 0B 01 nn Emergency code = 0B, watchdog error   = 02, undefined prescaler = nn 
 * 
 * Command
 * A5 01 11 B5    Ping
 * Telemetry
 * CA 01 11 DA    Pong 
 * 
 * Tip: In CoolTerm open multiple Send String windows ready to send...
 *      "Set watchdog interrupt for n sec", "Ping", and "Watchdog off" 
 *      To open multiple windows From the Menu Bar select "Connection" 
 *      and from the drop down menu "Send String..."
 */ 

/* 
 * Watchdog Timer Initialization
 * WDTCSR configuration:
 * bit                         7654_3210
 * 7 WDIF Interrupt Flag     = X
 * 6 WDIE Interrupt Enable   =  1
 * 5 WDP3 Prescaler bit 3    =   n
 * 4 WDCE Change Enable      =    0_
 * 3 WDE  System Reset Enable =     1     
 * 2..0   Prescaler bits =       n   nnn   all times assume Vcc = 5.0v (a little longer at 3.3v)         macro
 *        0                      0   000    2K (2048)    cycles / 128kHz internal oscillator = 16 msec   WDTO_15MS                         
 *        1                      0   001    4K (4096)    cycles / 128kHz internal oscillator = 32 msec   WDTO_30MS                         
 *        2                      0   010    8K (8192)    cycles / 128kHz internal oscillator = 64 msec   WDTO_60MS
 *        3                      0   011   16K (16384)   cycles / 128kHz internal oscillator = 0.125 sec WDTO_120MS 
 *        4                      0   100   32K (32768)   cycles / 128kHz internal oscillator = 0.25 sec  WDTO_250MS                           
 *        5                      0   101   64K (65536)   cycles / 128kHz internal oscillator = 0.5 sec   WDTO_500MS                          
 *        6                      0   110  128K (131072)  cycles / 128kHz internal oscillator = 1.0 sec   WDTO_1S
 *        7                      0   111  256K (262144)  cycles / 128kHz internal oscillator = 2.0 sec   WDTO_2S
 *        8                      1   000  512K (524288)  cycles / 128kHz internal oscillator = 4.0 sec   WDTO_4S
 *        9                      1   001 1024K (1048576) cycles / 128kHz internal oscillator = 8.0 sec   WDTO_8S
 *                               1   010 Reserved
 *                                 :   
 *                               1   111
 * WDTCSR configuration:    7654_3210                      
 * WDTCSR            0x00 = 0000_0000  // Watchdog Off
 *                   0xnn = 01n0_1nnn  // see table above
 *                   0x4D = 0100_1101  0.5 sec interrupt and system reset mode  
 *                   0x4E = 0100_1110  1 sec interrupt and system reset mode                          
 *                   0x4F = 0100_1111  2 sec  
 *                   0x68 = 0110_1000  4 sec                         
 *                   0x69 = 0110_1001  8 sec
 *                   0xnn = 01n0_0nnn  // see table above
 *                   0x46 = 0100_0110  1 sec interrupt mode                          
 *                   0x47 = 0100_0111  2 sec  
 *                   0x60 = 0110_0000  4 sec                         
 *                   0x61 = 0110_0001  8 sec
 *                   0x62 = 0110_0010  reserved
 *                          
 */

 /* How it Works
  * We are operating the watchdog timer in the "Interrupt and System Reset" mode, as defined 
  * in Table 10-1 (ATmega328P), with WDTON = 1, WDIE = 1, WDE = 1. The Arduino WDTON  
  * bit in the High Byte fuse register is set to 1 when the chip is configured. WDTCSR bits
  * WDE and WDIE are set to one when the watchdog timer prescaler is setup (watchdogSetup). 
  * WDTCSR = (1<<WDIE) | ((prescaler & 0x08)<<2) | (1<<WDE) | (prescaler & 0x07);
  * WTCSR bits WDP3 -> WDP0 are contained in the "prescaler" argument sent with the watchdogSetup  
  * call. As shown in Section 10.9.2 "WDTCSR – Watchdog Timer Control Register," these bits are  
  * unfortunately not contiguous. The value of these bits needed to setup delays of 4 and 8 seconds 
  * are defined in Table 10-2 "Watchdog Timer Prescale Select." The line of code above splits 
  * these bits up and inserts them into the WDTCSR as defined in Section 10.9.2. 
  * Once initialized, the processor must receive a wdt assembly instruction within the time period  
  * or an watchdog interrupt will be generated (Command A5 01 11 B5). When the interrupt occurs the hardware 
  * automatically clears the WDIE bit, thus putting the the watchdog timer into the "System Reset" 
  * mode, as defined in Table 10-1 (ATmega328P), with WDTON = 1, WDIE = 0, WDE = 1. Consequently,
  * a reset is generated. From observation, the reset seems to occur at the end of the ISR.
  * Example; assuming WDTCSR is configured for an 8 second delay, and you add a Serial.write(WDTCSR); 
  * instruction at the beginning of the Arduino setup procedure in the ino file (after setting
  * the Serial baud rate) and at the end of the watchdogSetup method and the ISR routine in Watchdog.h, 
  * you will see WDTCSR = 0x69 when the setup command is sent A5 02 10 08 BF, followed 8 seconds later by  
  * WDTCSR = 0x29, and finally WDTCSR = 0x00 after reset.
  */

/*
Latency Errors

What causes a latency errors and under what conditions does the robot go into a safe mode? 
To answer these questions 3 PING-PONG games need to be played.

PING-PONG Game #1 Phone vs. MCU
Phone sends a PING command and MCU sends a PONG back.  Phone checks the difference and sends 
Latency error to the Control Panel if latency period is exceeded. In current releases of the 
App, the phone no longer reacts drastically to an overdue PONG (early versions used to interpret 
this as a disconnection).

PING-PONG Game #2 Control Panel vs. Phone
When the Control Panel displays an emergency stop message for latency, it is caused by our other 
game of PING-PONG that goes on between Control Panel and Phone (disabled for "rovers" that can't 
"rove," such as ATechTop). 

Jeff - Is a stop message actually sent to the rover?

PING-PONG Game #3 Control MCU vs. Phone

On the MCU side, latency only represents a safety concern if the rover is in moving. Therefore, 
if robot detects a PING interval greater than that defined by the PING_INTERVAL, the robot will
stop the motors and send an EMERGENCY_ID = 0x0B with a WATCHDOG_TIMEOUT = 0x0100 argument. 

The PING_INTERVAL command is provided so that the user has the ability to adjust the interval 
from the Phone, rather than just having a hard-coded value.  This feature has not be implemented 
in the current version of the Application.

Jeff - In RC mode the PING should come from the phone. In non-RC mode the PING should come from 
the control panel and NOT the phone (which is on the rover and go off the cliff with it merrily 
pining away). Is this the current implementation?

SLEEP
Due to the potential to brick the 32U4 if sleep is not done correctly
sleeping the processor is not implemeneted at this time. After reviewing
documentation, direct register manipulations with some in-line assembly 
may be best. Recommend starting with Post #9 found here.  
http://www.avrfreaks.net/forum/solved-need-code-watchdog-interrupt-only-configuration

*/



