ArxRobot-Library
================

Library supporting Arxterra 3DoT board and communication with ArxRobot app/control panel

Installation
------------

Detailed instructions for installing:
https://www.arxterra.com/getting-started-with-3dot-initial-arduino-setup/?v=7516fd43adaa

Version History
--------------
```
v3.0 (03/04/2020)
* Reverted v2.0 Changes
* setCurrentLimit added to ArxRobot Class
* readBatteryVoltage and alertFatalError added to ArxRobot Class for checking critically low battery.
* Added preprocesser directives to distinguish between 3DoT versions
* Renamed "TB6612FNG.*" to "MotorDriver.*"
* Added servo3DoT headers

v2.0 (07/22/19)
* Fuelgauge.x renamed to PowerManagement.x
* setCurrentLimit added to PowerManagement
* Accompanying SET_CURRENTLIMIT telecom command added

v1.0 (03/12/19)
* Library renamed from "Robot3DoTBoard" to ArxRobot
* ArxRobot.cpp no longer waits until usb communication is initiated
* Telecomm example updated to be compatible with latest Arduino IDE
```