ArxRobot-Library Reference
==========================

Overview
------------------

The ArxRobot library allows the 3DoT board to communicate over bluetooth using **command** and **telemetry** `Data Packets`_ with checksum.

**Commands** are, as the name suggests, instructions for the robot or app to do something. **Telemetry** is simply data, such as the battery voltage that is communicated.

The library contains a list of `Built-In Commands`_ and telemetry labels, listed in Configure.h, such as the MOVE command, which is command number 01.

The function of these commands are defined in the library. For example, when the MOVE command is received, motors connected to the 3DoT are driven at the given speed and direction. 

------

ArxRobot.loop() checks for new commands received from the bluetooth module and runs them. Hence, placing it in the loop section of an Arduino sketch is enough to make a robot work with all the built-in commands and telemetry. 

The "ArxRobot_Basic" sketch demonstrates this basic functionality.

------

To add functionality to a robot, users can also add their own `Custom Commands`_ . The code to be executed by the command is written in the user's Arduino sketch, and handled by the ArxRobot Library and ArxRobot.loop(). 


Data Packets
------------

lorem ipsum dolor

Built-In Commands
-----------------

lorem ipsum dolor

Custom Commands
---------------

lorem ipsum dolor
