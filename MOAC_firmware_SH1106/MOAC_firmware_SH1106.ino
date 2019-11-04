/*
 http://www.arduino.cc/en/Tutorial/KeyboardMessage

 LeoStick pins:
 A0  Button 0
 A1  Button 1
 A2  Button 2
 A3  Button 3
 A4  Button 4
 A5  Button 5
 A6  Button 6
 A7  Button 7
 D2  SDA (I2C) for OLED
 D3  SCL (I2C) for OLED
 D11 Piezo
 D13 LED Red
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#include <Fonts/FreeSans24pt7b.h>

#define OLED_RESET 0  // Is this connected?
Adafruit_SH1106 display(OLED_RESET);

// Keypad scanning library to detect keypresses:
#include <Keypad.h>

// Software USB keyboard library to emulate a keyboard:
#include "Keyboard.h"


// Input setup
//byte inputPins[8] = {A0,A1,A2,A3,A4,A5,6,11};
byte inputPins[8] = {A0,A1,A2,A3,A4,A5,A6,A7};
// MISTAKE HERE! I connected to A11 (D6) but it's the wrong pin. Should be A7.


/* Macros to send
     info     show       group     move
     delete   change     add       replace
     name     value      line      text
     net      label      route     ripup    
     layer 1   layer 16  ratsnest  ripup @;
*/
char *macro_messages[] = {
  "a\n",/*info*/    "b\n",/*show*/     "c\n",/*group*/    "d\n",/*move*/
  "e\n",/*delete*/  "f\n",/*change*/   "g\n",/*add*/      "h\n"/*replace*/
};

uint16_t last_activity_time = 0;  // Milliseconds
uint8_t  display_timeout    = 3;  // Seconds

/**
 * Setup
 */
void setup() {
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  show_startup_screen();
  
  // initialize control over the keyboard:
  Keyboard.begin();

  Serial.begin(115200);
}

/**
 * Displayed at startup
 */
void show_startup_screen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Mini Open");
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println("Adaptive Controller");
  display.setCursor(0,20);
  display.println("v1.0");
  display.display();
}

/**
 * Loop
 */
void loop() {
  //byte key = keypad.getKey();
  scan_inputs();
  /*
  if (key){
    //Serial.println(key);
    //Serial.println(macro_messages[key]);
    Keyboard.print(macro_messages[key]);
    last_activity_time = millis();
    show_display();
  }
  */

  if((millis() - last_activity_time) > (1000 * display_timeout))
  {
    //display.dim(0);
    display.clearDisplay();
    display.display();
  }
}

/**
 * Read each of the inputs to check for button presses
 */
void scan_inputs()
{
  for (byte i = 0; i < sizeof(inputPins); i++){
    if(analogRead(inputPins[i]) < 127)
    {
      Keyboard.print(macro_messages[i]);
      last_activity_time = millis();
      show_event(i);
      delay(100);
    }
  }
}

/**
 * Displayed at startup
 */
void show_event(int key)
{
  display.setFont(&FreeSans24pt7b);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(50,40);
  display.print(macro_messages[key]);
  display.display();
}
