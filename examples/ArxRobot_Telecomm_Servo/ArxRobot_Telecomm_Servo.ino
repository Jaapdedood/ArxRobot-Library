/*
 *   Custom Servo Command Sample Arduino Code:
 *   Add a slider widget on Arxrobot app to move servo at will
 */

#include <ArxRobot.h>     // instantiated as ArxRobot at end of class header

ArxRobot ArxRobot;       // instantiated as ArxRobot at end of class header

Servo servoA;                   // Instantiate new Servo object

/*
 * Command Example
 * Step 1: Assign new command mnemonics ID numbers
 *         In this example we will be adding just 1 custom command.
 */
#define SERVO       0x41

const uint8_t CMD_LIST_SIZE = 1;   // we are adding 1 command, Servo

/*
 * Command Example
 * Step 4: Write command handlers
 */
 
/*
 * User Defined Command SERVO (0x41) Example
 * Rotate servo to 90 degrees
 * A5 02 41 90 76
 */
bool servoHandler (uint8_t cmd, uint8_t param[], uint8_t n)
{
    // Serial.writes useful for debugging. Going to comment them out for now.
    /* Serial.write(cmd);             // servo command = 0x41
    Serial.write(n);                  // number of param = 1
    for (int i=0;i<n;i++)             // param = 90 degrees
    {
        Serial.write (param[i]);
    }*/

    /* From servo datasheet: "Position '-90' (1ms pulse), Position '90' (2ms pulse)"
     *  Using unsigned byte as widget data type in app, send value from phone 0-255
     *  and map to angle -90 to 90
     *  Could avoid using map() if data type short is used
    */
    servoA.writeMicroseconds(map(param[0], 0, 255, 1000, 2000));
    
    return false;  // return false since we are not intercepting a predefined command
}  // servoHandler

/*
 * Command Example
 * Step 2: Register the SERVO commands by linking its ID to the corresponding handler.
 */

ArxRobot::cmdFunc_t onCommand[CMD_LIST_SIZE] = {{SERVO,servoHandler}};


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

  ArxRobot.setCurrentLimit(0);    // set current limit to max current ~= 500mA per motor
                                  // to set a lower current limit, see https://www.arxterra.com/current-limit/
}

void loop()
{
  ArxRobot.loop();
}
