/*
 *   Custom Servo Command Sample Arduino Code:
 *   Add two slider widgets (unsigned int 0 - 255) on Arxrobot app to move servos at will
 *   Add a third slider widget (unsigned int 1 - 255) to set the speed of the servos
 */

#include <ArxRobot.h>     // instantiated as ArxRobot at end of class header

ArxRobot ArxRobot;       // instantiated as ArxRobot at end of class header

Servo3DoT servoA;                   // Instantiate new Servo object
Servo3DoT servoB;

/*
 * Command Example
 * Step 1: Assign new command mnemonics ID numbers
 *         In this example we will be adding just 1 custom command.
 */
#define SERVOA       0x41
#define SERVOB       0x42
#define SERVOSPEED   0x43


const uint8_t CMD_LIST_SIZE = 3;   // we are adding 3 commands

// Store the servo speed globally
uint8_t g_servoSpeed = 0;

/*
 * Command Example
 * Step 4: Write command handlers
 */
 
/*
 * User Defined Command SERVO (0x41) Example
 * Rotate servo to 90 degrees
 * A5 02 41 90 76
 */
bool servoHandlerA (uint8_t cmd, uint8_t param[], uint8_t n)
{
    /* write(angle), write(angle, speed)
     *  angle = angle between 0, 180
     *  speed = 0 -> default speed
     *  speed = 1 -> minimum speed
     *  speed = 255 -> maximum speed
     */
    servoA.write(param[0], g_servoSpeed);
    
    return false;  // return false since we are not intercepting a predefined command
}  // servoHandler


bool servoHandlerB (uint8_t cmd, uint8_t param[], uint8_t n)
{
    servoB.write(param[0], g_servoSpeed);
    
    return false;  // return false since we are not intercepting a predefined command
}  // servoHandler

bool setServoSpeed(uint8_t cmd, uint8_t param[], uint8_t n)
{
  g_servoSpeed = param[0];
}
/*
 * Command Example
 * Step 2: Register the SERVO commands by linking its ID to the corresponding handler.
 */

ArxRobot::cmdFunc_t onCommand[CMD_LIST_SIZE] = {{SERVOA,servoHandlerA}, {SERVOB, servoHandlerB}, {SERVOSPEED, setServoSpeed}};


void setup()
{
  Serial.begin(9600);             // default = 115200
  ArxRobot.begin();

  /*
  * Command Example
  * Step 3: Tell 3DoT Robot software about new commands
  *         and perform any necessary setup.
  */
  ArxRobot.setOnCommand(onCommand, CMD_LIST_SIZE);

  servoA.attach(SERVO_A);         // Attach servo. SERVO_A and SERVO_B are defined in ArxRobot.h
  servoB.attach(SERVO_B);         // Attach servo. SERVO_A and SERVO_B are defined in ArxRobot.h

  ArxRobot.setCurrentLimit(60);    // Set Current limit to ~500mA
}

void loop()
{
  ArxRobot.loop();
}
