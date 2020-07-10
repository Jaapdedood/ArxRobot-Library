The 3DoT Board
==============

Battery
-------

When you first get your board, the battery will be at an ~80% charge, or about 3.7V. Plug in a usb cord connected to a computer or USB wall charger to charge the battery. The 3DoT Board will automatically stop charging at 4.2V, the maximum safe charge voltage.

.. attention::
   
   Do not let the battery discharge below 3.2V, as this could cause permanent damage to the battery's chemistry and significantly reduce battery life.

ON - PRG - RUN Switch
---------------------

Next to the USB port, the 3-position switch on the 3DoT board is designed to allow you to set the board to programming mode, take your time programming the board, and then switch the board to ON when you are ready to run the code.

.. note::

   In order to go from ON to programming the board, you must first turn the board OFF, then switch back to PRG.

.. figure:: https://www.arxterra.com/wp-content/uploads/2018/04/Getting-Started-SW-square-e1572375117922.png
    :align: center

    ON - PRG -OFF Switch
		

Motors
------

The 3DoT motor driver supplies 0-5V to the on-board motor connectors. Any 3-6V motors will work, as long as they are not too large (no-load current above ~200mA).

We highly recommend `micro-metal gear motors <https://www.humansforrobots.com/product/micro-metal-gear-motor-6v-200rpm/>`__.

Servos
------

Servos on the 3DoT are powered from the same VM net as the motor drivers. Again, any 3-6V servos should work, but be careful with amount of current that can be drawn from the on-board battery.

We recommend any kind of "micro-servo".

External Battery Connector
--------------------------

Depending on the motors and servos used, the amount of torque required from them, current drawn by shields etc. you may hit the 3DoT battery's 2A limit. You can alleviate this problem by desoldering the marked solder bridge on the bottom of the board and connecting an external battery or other power supply (4V - 18V) to the external battery connector.This will allow the internal electronics of the board to run from the on-board battery, and supply your battery's power directly to the "VM" net, which is accessible from the top header, and powers the motor driver. 

Shields
-------

Shields add functionality to your 3DoT board, such as extra motor connectors or sensors, and are available to purchase on the `Humans for Robots store <https://www.humansforrobots.com/store/>`__, or your can make your own.

To make your own, try free PCB CAD software such as KiCad or EAGLE and use the 3DoT PCB CAD files below as a reference.

PCB CAD Files
-------------

Updated files to be released soon.
