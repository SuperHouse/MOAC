Mini Open Adaptive Controller firmware
======================================

Firmware examples for the Mini Open Adaptive Controller. We have
experimented with different OLEDs and libraries during development,
so this is a collection of the different firmware versions.

The most recent hardware uses a 128x64 OLED with the SH1106 driver.

Unfortunately the various SH1106 drivers are a bit strange, and the
best I've found is the version of Adafruit's SSD1306 library that
was forked by wonho-maker and modified to add SSD1306 support. That
version is not available through the Arduino library manager, so
it's been embedded directly within the project.

https://www.superhouse.tv/moac
