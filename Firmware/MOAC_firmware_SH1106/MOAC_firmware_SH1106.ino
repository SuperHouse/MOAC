/**
  Mini Open Adaptive Controller firmware.

  Read digital inputs and send keyboard, mouse, joystick, or game controller
  events to a host computer via USB. This allows custom input devices to be
  created using buttons or other specialised input devices.

  Includes a driver for a 1.3" 128x64 pixel monochrome OLED display with
  I2C interface, so that input events can be displayed on screen.

  This firmware is for the SuperHouse Mini Open Adaptive Controller, but also
  works just as well on an Arduino Leonardo so you can build your own
  hardware to suit your own needs.

  Compile options:
      Tools -> Board -> Arduino AVR Boards -> Arduino Leonardo

  External dependencies. Install using the Arduino library manager:
      "Adafruit GFX Library" by Adafruit
      "Keypad" by Mark Stanley, Alexander Brevig
      "HID-Project" by Nico Hood

  Bundled dependencies. No need to install separately:
      "Adafruit SH1106" by wonho-maker, forked from Adafruit SSD1306 library

  More information:
    www.superhouse.tv/moac

  To do:
      Joystick, mouse, and game controller events
      Configurable auto repeat

  Written by Jonathan Oxer for www.superhouse.tv
    https://github.com/superhouse/MOAC

  Copyright 2020-2021 SuperHouse Automation Pty Ltd www.superhouse.tv
*/
#define VERSION "3.0"
/*--------------------------- Configuration ------------------------------*/
// Configuration should be done in the included file:
#include "config.h"

/*--------------------------- Libraries ----------------------------------*/
#include <SPI.h>                      // For OLED
#include <Adafruit_GFX.h>             // For OLED
#include "Adafruit_SH1106.h"          // For OLED
#include <Keypad.h>                   // To read button inputs
#include "HID-Project.h"              // HID library with absolute mouse positioning

/*--------------------------- Global Variables ---------------------------*/
uint16_t g_last_activity_time   = 0;  // Milliseconds
uint8_t  g_warp_location_index  = 0;
uint8_t  g_warp_location_count  = 0;

/*--------------------------- Resources ----------------------------------*/
#include <Fonts/FreeSans24pt7b.h>     // For OLED

/*--------------------------- Function Signatures ------------------------*/
void showStartupScreen();
void scanInputs();
void screensaver();
void showEvent(uint8_t button_index);

/*--------------------------- Instantiate Global Objects -----------------*/
Adafruit_SH1106 display(OLED_RESET);

/*--------------------------- Program ------------------------------------*/
/**
  Setup
*/
void setup()
{
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C address 0x3C
  showStartupScreen();

  g_warp_location_count = (sizeof(warp_locations) / sizeof(warp_locations[0]));

  // Initialise a virtual USB keyboard:
  Keyboard.begin();

  // Initialise a virtual mouse in "absolute position" mode:
  AbsoluteMouse.begin();

#if ENABLE_BEEP
  pinMode(PIEZO_PIN, OUTPUT);
  tone(PIEZO_PIN, BEEP_TONE, BEEP_DURATION);
#endif
}

/**
  General info screen displayed at startup
*/
void showStartupScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.println("      Mini Open");
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.println(" Adaptive Controller");
  display.setCursor(50, 25);
  display.print("v");
  display.print(VERSION);
  display.setCursor(10, 40);
  display.println("superhouse.tv/moac");
  display.display();
}

/**
  Loop
*/
void loop()
{
  scanInputs();
  screensaver();
}

/**
  Read each of the inputs to check for button presses
*/
void scanInputs()
{
  for (uint8_t i = 0; i < sizeof(g_input_pin_list); i++) {
    //if (analogRead(g_input_pin_list[i]) < 127) // Using analogRead while experimenting
    if (LOW == digitalRead(g_input_pin_list[i]))
    {
      if (g_macro_messages[i] == "warp")
      {
        // Do mouse stuff
        // Move the cursor to the top left corner.
        AbsoluteMouse.moveTo(0, 0);
        delay(100);  // Test to give mouse location time to settle in host OS and do basic debounce

        // Jump to the target location:
        int16_t x_target_location = (int)warp_locations[g_warp_location_index][0] * 32.768;
        int16_t y_target_location = (int)warp_locations[g_warp_location_index][1] * 32.768;

        // Select the next location from the list, ready for next time:
        g_warp_location_index ++;
        if (g_warp_location_index >= g_warp_location_count)
        {
          g_warp_location_index = 0;
        }

        AbsoluteMouse.move(x_target_location, y_target_location);
        showEvent(i);
        delay(50);
      } else {
        // Do keyboard stuff
        Keyboard.print(g_macro_messages[i]);
#if ENABLE_BEEP
        tone(PIEZO_PIN, BEEP_TONE, BEEP_DURATION);
#endif
        g_last_activity_time = millis();
        showEvent(i);
        delay(50);
      }
    }
  }
}

/**
  Blank the screen if the timeout period has been reached
*/
void screensaver()
{
  if (millis() > (g_last_activity_time + (1000 * DISPLAY_TIMEOUT)))
  {
    display.clearDisplay();
    display.display();
  }
}

/**
  Display the event on the screen
*/
void showEvent(uint8_t button_index)
{
  display.setFont(&FreeSans24pt7b);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(50, 40);
  display.print(g_macro_messages[button_index]);
  display.display();
}
