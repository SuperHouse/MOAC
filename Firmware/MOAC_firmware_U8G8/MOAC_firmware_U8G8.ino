/*
 http://www.arduino.cc/en/Tutorial/KeyboardMessage

 Driver library:
 https://github.com/olikraus/u8g2

 Sends Eagle macros for PCB design
 
 LeoStick pins:
 A0  Button 00
 
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
 D13 LED Red0
 */

//#include <SPI.h>
#include <Wire.h>

#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Keypad scanning library to detect keypresses:
#include <Keypad.h>

// Software USB keyboard library to emulate a keyboard:
#include "Keyboard.h"

// Input setup
byte inputPins[8] = {A0,A1,A2,A3,A4,A5,6,11};
// MISTAKE HERE! I connected to A11 (D6) but it's the wrong pin. Should be A7.


/* Macros to send
     info     show       group     move
     delete   change     add       replace
     name     value      line      text
     net      label      route     ripup    
     layer 1   layer 16  ratsnest  ripup @;
*/
char *macro_messages[] = {
  "0\n",/*info*/    "1\n",/*show*/     "2\n",/*group*/    "3\n",/*move*/
  "4\n",/*delete*/  "5\n",/*change*/   "6\n",/*add*/      "7\n"/*replace*/
};

uint32_t last_activity_time = 0;  // Milliseconds
uint8_t  display_timeout    = 5;  // Seconds
char* last_keypress = "";

/**
 * Setup
 */
void setup() {
  u8g2.begin();
  show_startup_message();
  
  // initialize control over the keyboard:
  Keyboard.begin();

  Serial.begin(115200);
}

/**
 * Un-blank the display
 */
void show_display()
{
  u8g2.clearDisplay();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso62_tn);
  //display.drawString(0, 0, "ABCdefg");
  u8g2.drawStr(32, 64, last_keypress);
  u8g2.sendBuffer();
  //display.drawString(0, 4, "v1.0");0
}

/**
 * Shown on first startup
 */
void show_startup_message()
{
  ////display.clearDisplay();
  //u8g2.setFont(u8x8_font_chroma48medium8_r);
  //u8g2.drawStr(0, 12, "Mini Open");
  //u8g2.drawStr(0, 24, "Adaptive");
  //u8g2.drawStr(0, 36, "Controller");
}

/**
 * Loop
 */
void loop() {
  //byte key = keypad.getKey();
  scan_inputs();
  //show_display();

  /*
  if (key){
    //Serial.println(key);
    //Serial.println(macro_messages[key]);
    Keyboard.print(macro_messages[key]);
    last_activity_time = millis();
    show_display();
  }*/
  

  if((millis() - last_activity_time) > (1000 * display_timeout))
  {
    //display.dim(0);
    u8g2.clearDisplay();
    //display.display();
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
      last_keypress = macro_messages[i];
      last_activity_time = millis();
      show_display();
      //delay(100); // Crude debounce. Will make the inputs laggy. Need to add better auto-repeat
    }
  }
}
