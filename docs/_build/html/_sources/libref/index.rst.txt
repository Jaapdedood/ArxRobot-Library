ArxRobot-Library
================

Make sure you have completed the `Getting Started <../arduino.html>`__ section before diving into this section.

Overview
------------------

The ArxRobot library allows the 3DoT board to communicate over bluetooth using **command** and **telemetry** `Data Packets`_ with checksum.

**Commands** are, as the name suggests, instructions for the robot or app to do something. **Telemetry** is simply data, such as the battery voltage that is communicated.

The library contains a list of `Built-In Commands`_ and telemetry labels, listed in Configure.h, such as the MOVE command, which is command number 01.

The function of these commands are defined in the library. For example, when the MOVE command is received, motors connected to the 3DoT are driven at the given speed and direction. 

Example Sketches
----------------

In the Arduino IDE, navigate to `File > Examples > ArxRobot_Library` to open the included example sketches.


**ArxRobot_Basic** contains all the code needed to control a robot using the RoboPilot app.


**ArxRobot_CustomCommands** implements the addCustomCommand() and replaceBuiltInCommand() methods explained below to make user commands work with the RoboPilot app.

**ArxRobot_ledPWM** shows a simple custom command example, using a slider to adjust the brightness of an LED

Writing Custom Commands
-----------------------

To get started making a custom command, write a function in your arduino sketch with the following template:

.. code-block:: c++

		void myCommand (uint8_t cmd, uint8_t param[], uint8_t n)
		{
		// Your code here
		}

You can change myCommand to the name of your choosing, e.g. blinkLED, but the type must always be **void** and parameters must follow the **(uint8_t cmd, uint8_t param[], uint8_t n)** format.

You can use these parameters in your function.

.. cpp:function:: void myCommand(uint8_t cmd, uint8_t param[], uint8_t n)

    :param cmd: The command ID of your custom command
    :param param[]: Command data sent by the app
    :param n: Number of bytes of command data received in param[]

For example, in the ArxRobot_ledPWM example we adjust the brightness of an LED using the slider value sent from the RoboPilot app as follows:

.. code-block:: c++
		
		void ledPWM (uint8_t cmd, uint8_t param[], uint8_t n)
		{
		    // param[0] is the first byte of command data received from the app,
		    // which holds the slider value
		    analogWrite(LED, param[0]);
		}
		
Adding Custom Commands
----------------------

Next, to add the function as a custom command that the app can call, simply call the following function in the setup portion of your code:

.. cpp:function:: void ArxRobot::addCustomCommand(fptr_t function, uint8_t commandID)

    :param function: Name of function to add to custom commands list
    :param param[]: Command ID to associate with function

For example:

.. code-block:: c++

		void setup()
		{
		    ArxRobot.begin();
		
		    ArxRobot.addCustomCommand(myCommand, 0);
		    ArxRobot.addCustomCommand(myOtherCommand, 1);
		}

In the setup() of this sketch, we are adding "myCommand" and "myOtherCommand" as two custom commands, with IDs 0 and 1.

If you now create a custom command in the RoboPilot app with ID 0, it will call myCommand, and a command with ID 1 will call myOtherCommand.

Built-In Commands
-----------------



Replacing Built-In Commands
---------------------------



Data Packets
------------



Custom Commands
---------------



Library Reference
-----------------

.. toctree::
   :maxdepth: 1

   Configure <configure>
   ArxRobot <arxrobot>
   Motor/Motor Driver <motor>
   Packet <packet>
   Servo3DoT <servo>
   Telecom <telecom>
   Watchdog <watchdog>
      
