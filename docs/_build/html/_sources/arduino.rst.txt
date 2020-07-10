Getting Started
===============

The Arduino IDE is the most popular beginner method for programming compatible microcontrollers.

Download the Arduino IDE from the Arduino website:

https://www.arduino.cc/en/Main/Software

Adding the 3DoT Board to Arduino
--------------------------------

1. Navigate to File / Preferences

.. image:: https://www.arxterra.com/wp-content/uploads/2018/09/Getting-Started-3DoT-IDE-preference-window.png
    :alt: preferences pane
   
2. Paste the following link into Additional Board Manager URLs:
   https://arxterra.com/downloads/package_3DoT_index.json
   
3. Close the preferences window and navigate to Tools / Board: / Boards Manager…

.. image:: https://www.arxterra.com/wp-content/uploads/2018/09/Getting-Started-3DoT-board-manager.png
    :alt: boards manager
		 
4. Type “3DoT” in the search bar. The board Should show up.

.. image:: https://www.arxterra.com/wp-content/uploads/2018/09/bootloader4.png
    :alt: 3DoT in boards manager
   
5. Click inside of the box that contains the text “3DoT by Arxterra” and the “Install” button should appear. Click Install.

.. image:: https://www.arxterra.com/wp-content/uploads/2018/09/bootloader5.png
    :alt: 3DoT installed in boards manager

Installing the 3DoT Arduino Library
-----------------------------------

In the Arduino IDE, navigate to

Sketch > Include Library > Manage Libraries..

Search for ArxRobot Library and click install. Done!

.. image:: https://www.arxterra.com/wp-content/uploads/2018/04/Screenshot-from-2020-03-24-09-59-41.png
    :alt: ArxRobot Library in Arduino library manager

Uploading Your First Sketch
---------------------------

With the Library Installed, navigate to File > Examples > ArxRobot Library and click on ArxRobot_Basic

.. image:: https://www.arxterra.com/wp-content/uploads/2018/04/examples.png
    :alt: Basic Example location

The ArxRobot_Basic example is a barebones sketch that handles all the background robot operations, including Bluetooth commands. **This means that uploading the Basic example code will make your robot ready to connect to Bluetooth and go!**


Select a COM port
^^^^^^^^^^^^^^^^^^^^

1. Set the 3DoT to PRG mode and connect your 3DoT to your computer with a USB cable

2. Navigate to Tools > Port and select the port labeled as a 3DoT board.

.. image:: https://www.arxterra.com/wp-content/uploads/2018/04/port.jpg
    :alt: 3Dot com port


Then, simply hit the Upload button in the Arduino IDE.

.. note::
   If a sketch fails to upload, check the COM port selected again. Your computer may change the COM port number as the board is turned off/on

Next Steps
----------

Congratulations! You uploaded your first sketch to the 3DoT Board. Click the next button to learn how to send commands to your 3DoT using the RoboPilot app.

