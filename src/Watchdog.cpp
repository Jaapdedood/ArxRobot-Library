/*
  Watchdog.cpp - Library for monitoring the communication link
  with the application/control panel and the robot.
  Written by Gary Hill, August 13, 2016
*/

#include "Arduino.h"     // Arduino library files
#include <avr/wdt.h>     // Standard C library for AVR-GCC avr-libc Arduino\hardware\tools\avr\avr\include\avr\wdt.h
#include "Packet.h"      // packetize and send data to the 3DoT application => Arxterra control panel
#include "TB6612FNG.h"   // TB6612FNG Motor Driver
#include "Watchdog.h"

// instantiate objects
Packet wdtPacket(EMERGENCY_ID);  // EMERGENCY_ID duplicated in call
TB6612FNG motorDriver;

/*
 *  C++ .cpp member class definitions
 */

/*
 * Watchdog Timer Interrupt
 * Be careful not to use functions they may cause the interrupt to hang and
 * prevent a reset.
 */
ISR(WDT_vect) //
{
  // Safe 3DoT
  motorDriver.motors_safe();
  wdtPacket.sendPacket(EMERGENCY_ID,WATCHDOG_TIMEOUT);    // send EMERGENCY_ID 0x0B with WATCHDOG_TIMEOUT code 0x0100
}

// Constructor
Watchdog::Watchdog()
{
}

/*
 * Watchdog Setup
 * while watchdogSetup else block is roughly equivalent to wdt_enable
 * macro in wdt.h as discussed in title block.
 * alternate macros shown in comments follow IAR not AVR-GCC conventions
 * http://www.nongnu.org/avr-libc/user-manual/porting.html
 */
void Watchdog::watchdogSetup(uint8_t mode_prescaler)
{
  _prescaler = ((mode_prescaler & 1<<WDP3)>>2) | (mode_prescaler & 0x07);       // extract prescaler WDP3..WDP0
  _mode = (((1<<WDIE) & mode_prescaler)>>5) | (((1<<WDE) & mode_prescaler)>>3); // extract mode WDIE:WDE
  /* Serial.write(_prescaler); */
  /* Serial.write(_mode); */
  if (mode_prescaler == 0x00){
    watchdogOff();    // turn off watchdog timer
  }
  else if ((WDTO_1S <= _prescaler) && (_prescaler <= WDTO_8S)) // only allowable prescale values
  {
    cli();              // __disable_interrupt();
    wdt_reset();        // __watchdog_reset(); included in avr/wdt.h library, assembly instruction wdr
    // enter Watchdog Configuration mode
    // keep old prescaler setting to prevent unintentional time-out
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    // Interrupt and System Reset mode (see Table 10-1) plus Prescaler (see Table 10-2)
    // timed instuction (4 cycles max)
    WDTCSR = mode_prescaler;
    sei();             // __enable_interrupt();
  }
  else
  {
    throwError(word(0x06,mode_prescaler)); // send 0x0E with code 0x06 plus undefined argument
  }
}

/*
 * Private Methods
 */

/*
 * Watchdog Off
 * equivalent to wdt_off macro in wdt.h
 * provided code is from datasheet example
 * alternate macros shown in comments follow IAR not AVR-GCC conventions
 * http://www.nongnu.org/avr-libc/user-manual/porting.html
 */
void Watchdog::watchdogOff(void)
{
  cli();              // __disable_interrupt();
  wdt_reset();        // __watchdog_reset();
  // Clear WDRF in MCUSR
  MCUSR &= ~(1<<WDRF);
  // Write logical one to WDCE and WDE
  // Keep old prescaler setting to prevent unintentional time-out
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Turn off WDT
  WDTCSR = 0x00;
  sei();             // __enable_interrupt();
}

void Watchdog::throwError(uint16_t err){
  wdtPacket.sendPacket(EXCEPTION_ID,err);        // send 0x0E plus 16-bit FSM error code
  #if DEBUG
  Serial.print("Watchdog exception 0x0");        // Send duplicate data as text to
  Serial.println(err,HEX);                       // USB=>Arduino IDE Serial Monitor.
  #endif
}
