/*
 * Telemetry and Custom Command Sample Arduino Code
 *   Telemetry Example: Monitor an extended I/O register value and send to control panel display
 *   Command Example:   Add Robot unique MOVE, BLINK, and SERVO commands
 */

// include C:\Program Files (x86)\Arduino 1_6_5\hardware\arduino\avr\libraries
#include <ArxRobot.h>     // instantiated as ArxRobot at end of class header
#include <EEPROM.h>
#include <Wire.h>               // I2C support

ArxRobot ArxRobot;       // instantiated as ArxRobot at end of class header

/*
 * Command Example
 * Step 1: Assign new command mnemonics ID numbers
 *         In our example we will be adding 3 custom commands (2 new and one predefined).
 *         The predefined MOVE command is intercepted and our robot unique code is to be
 *         run instead. The MOVE command mnemonic ID 0x01 is already defined in Configure.h
 *         The next two commands are new and assigned to the first two addresses in the
 *         custom command address space 0x40 - 0x5F.
 */
#define BLINK       0x40
#define SERVO       0x41
const uint8_t CMD_LIST_SIZE = 3;   // we are adding 3 commands (MOVE, BLINK, SERVO)

/*
 * Command Example
 * Step 4: Write command handlers
 */

/*
 * User Defined Command BLINK (0x40) Example
 * A5 01 40 E4
 */
void blinkHandler (uint8_t cmd, uint8_t param[], uint8_t n)
{
    Serial.write(cmd);                // blink command = 0x40
    Serial.write(n);                  // number of param = 0
    for (int i=0;i<n;i++)             // param = none
    {
        Serial.write (param[i]);
    }
}  // blinkHandler

/*
 * Override MOVE (0x01) Command Example
 * A5 05 01 01 80 01 80 A1
 */
void moveHandler (uint8_t cmd, uint8_t param[], uint8_t n)
{
    Serial.write(cmd);             // move command = 0x01
    Serial.write(n);               // number of param = 4
    for (int i=0;i<n;i++)          // param = 01 80 01 80
    {
        Serial.write (param[i]);
    }
}  // moveHandler

/*
 * User Defined Command SERVO (0x41) Example
 * Rotate servo to 90 degrees
 * A5 02 41 90 76
 */
void servoHandler (uint8_t cmd, uint8_t param[], uint8_t n)
{
    Serial.write(cmd);             // servo command = 0x41
    Serial.write(n);               // number of param = 1
    for (int i=0;i<n;i++)          // param = 90 degrees
    {
        Serial.write (param[i]);
    }
}  // servoHandler

/*
 * Command Example
 * Step 2: Register commands by linking IDs to their corresponding command handlers
 *         In our example when the MOVE command is intercepted the moveHandler is to be run.
 *         In a similar fashion the BLINK command calls the blinkHandler and SERVO the
 *         servoHandler.
 */

ArxRobot::cmdFunc_t onCommand[CMD_LIST_SIZE] = {{MOVE,moveHandler}, {BLINK,blinkHandler}, {SERVO,servoHandler}};

/*
 * Telemetry Example
 * Step 1: Instantiate packet
 *         In our example we simulate a current sensor wired to MOTOR 2. MOTOR2_CURRENT_ID
 *         is defined as 0x02 in Configure.h
 *         To simulate the data stream coming from the sensor we will read ATmega32U4
 *         Register OCR4D which controls the duty cycle of MOTOR 2.
 */
Packet motorPWM(MOTOR2_CURRENT_ID);  // initialize the packet properties to default values

void setup()
{
  Serial.begin(9600);               // default = 115200
  ArxRobot.begin();

  /*
  * Command Example
  * Step 3: Tell 3DoT Robot software about new commands
  */
  ArxRobot.setOnCommand(onCommand, CMD_LIST_SIZE);

  /* Telemetry Example
  * Step 2: Modify default values assigned to internal properties as needed.
  *         Before a packet is created and sent, it is qualified. Specifically,
  *         the data in a packet must change by some amount from the previous
  *         packet and may not be sent with at a period less than some value.
  *         In most cases you can leave these values at their default values.
  */
  motorPWM.setAccuracy(1);          // change sensor accuracy from +/-2 DN to +/-1 DN    (-- this line is optional --)
  motorPWM.setSamplePeriod(500);    // change sample period from 1 second to 0.5 seconds (-- this line is optional --)
}

void loop()
{ArxRobot.loop();

    /*
     * Telemetry Example
     * Step 3: Read sensor and send packet
     *         To simulate the data stream coming from the sensor we will read ATmega32U4
     *         Register OCR4D which controls the duty cycle of MOTOR 2. This code segment
     *         uses the preproccessor conditional directives #if to make sure that an MCU
     *         with a ATmega32U4 or ATmega16U4 is selected under Tools > Board.
     */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    uint16_t pwm_reading = (uint16_t) OCR4D;  // read 8-bit Output Compare Register Timer 4D and cast to 16-bit signed word
    motorPWM.sendSensor(pwm_reading);
  #else
    // Timer/Counter 0 registers set by UNO Bootloader (freq. = 1 Khz)
    // Timer/Counter Control Registers TCCR0B = 0x03 (Prescaler of 64) and TCCR0A (Fast PWM, TOP = 0xFF) = 0x03
    uint16_t pwm_reading = (uint16_t) OCR0B;
    motorPWM.sendSensor(pwm_reading);
  #endif
}
