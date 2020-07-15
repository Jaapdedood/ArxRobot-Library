RoboPilot
=========

Download
--------

Download the RoboPilot app on the Google Play Store:

https://play.google.com/store/apps/details?id=com.HumansforRobots.RoboPilot&hl=en_US

or from the App Store:
##

Connecting to Your Robot
------------------------

Ensure the ArxRobot_Basic sketch, or your own similar code, is uploaded to the 3DoT Board (see previous section).
   
Turn on the 3DoT Board and load up the app.
   
Tap "Scan" to look for your 3DoT. If nothing shows up, tap it again after a few seconds.
   
Click "Connect".
   
Once connected, try out the default controls to drive your robot's motors!

Reverse Motor Directions
------------------------

Click the "Reverse Motors" button from any of the controls screens to reverse any of the motor directions, in case your robot is running backwards or rotating when you try to drive it forward.

The toggles in this menu alter only the direction data byte in the command packet sent to the robot.

Adding Custom Commands
----------------------

Click the "Add Command" button from any of the controls screens and choose the type of command you wish to send.

The data packet is constructed by the app as follows for each command:

=============  =================  ======  =================  ===========================
Command Type   Data size (Bytes)  Format  Example             Notes
=============  =================  ======  =================  ===========================
Button              0                                         No data, just command sent
Toggle              1              00/01   01                 Boolean
Slider              1              0-125   5F
Text            maximum 20         ASCII   68 65 6c 6c 6f  
=============  =================  ======  =================  ===========================

You only need to worry about this once you start programming the robot's responses to the commands, covered on the next page: `ArxRobot Library Reference <#>`__.  

Saving and Loading Commands
---------------------------

Once you have constructed your perfect Robot UI, you can save it by clicking "Save Commands". When ever you want, you can then load them again by clicking "Load Commands". The D-pad and tank control screens save their commands in separate files and won't interfere with each other.

Support for saving multiple command layouts for each screen is a planned feature.


