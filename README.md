Mini Open Adaptive Controller
==============================
Copyright 2018-2020 SuperHouse Automation Pty Ltd  www.superhouse.tv  

Universal input device that allows external switches, buttons, etc
to be connected using 3.5mm mono plugs, and then emulate a USB
keyboard, mouse, joystick, or game controller to send HID events
to a computer or game console.

![Mini Open Adaptive Controller](Images/MOAC-v1_1-oblique.jpg)

Based on the ATmega32u4 microcontroller, compatible with the Arduino
Leonardo.

Features:

 * ATmega 32u4 MCU
 * 8 switch inputs (active low: short to activate)
 * HWB (hardware boot) button and solder jumper to allow disabling bootloader
 * 128x64 OLED module (SH1106)
 * USB-C connector
 * Piezo beeper
 * Beep on/off switch

You can view more details at:

  http://www.superhouse.tv/moac


Hardware
--------
The "Hardware" directory contains the PCB design as an EAGLE project.
EAGLE PCB design software is available from Autodesk free for
non-commercial use.


Firmware
--------
The "Firmware" directory contains multiple versios. We have experimented
with different OLEDs and libraries during development, so this is a
collection of the different firmware versions.

The most recent hardware uses a 128x64 OLED with the SH1106 driver.

Unfortunately the various SH1106 drivers are a bit strange, and the best
I've found is the version of Adafruit's SSD1306 library that was forked
by wonho-maker and modified to add SSD1306 support. That version is not
available through the Arduino library manager, so it's been embedded
directly within the project.


Credits
-------
Jonathan Oxer jon@oxer.com.au


License
-------
The hardware portion of this project is licensed under the TAPR Open
Hardware License (www.tapr.org/OHL). The "license" folder within this
repository also contains a copy of this license in plain text format.

