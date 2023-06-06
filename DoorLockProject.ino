/*
  Door Lock System Project
  A simple door lock project was created using Arduino UNO, servo motor, and lcd display.

  How its works?
  - Keypad key "*" is used to clear or reset password
  - Keypad key "#" is used to enter input password 
  - If the "WRONG PASSWORD" word appear on LCD, press the "*" keypad key to reset the password
  - If the "DOOR IS LOCKED" word appear on LCD, enter the password and press "#" to unlock
  - If the "DOOR IS UNLOCKED" word appear on LCD, enter the password and press "#" to lock
 
  The circuit:
  - Connect keypad row pinouts of the keypad to 5, 4, 3, and 2 pins of Arduino UNO
  - Connect keypad column pinouts of the keypad to 8, 7, and 6 pins of Arduino UNO
  - Connect LCD 1602 I2C SDA and SCL pins to Arduino UNO SDA and SCL pins
  - Connect Servor motor OUT pin to Arduino UNO pin 9. Then, connect GND and VCC pins to ground and +5V

  created 2021
  Created by ArduinoGetStarted.com
  Modified by My Creative Engineering <https://my-creative-engineering.blogspot.com>
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h> 

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad
const String password = "2323"; // change your password here
String input_password;
int position = 1;

Servo doorMotor; // create servo object to control a servo
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  doorMotor.attach(9);
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("DOOR LOCK SYSTEM");
  input_password.reserve(32);
  delay (200);
  lockPosition(true);
}

void loop()

{  
  while (position == 1)
  {
    int a;
    char key = keypad.getKey();

    if (key)
    {
 
        if(key == '*') // clear input password
        {
          input_password = "";
          lockPosition(true);
        } 
 
        else if(key == '#') // enter input password
        {
          if(password == input_password) 
          {
            lockPosition(false);
            position=0;     
          } 
      
          else 
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("WRONG PASSWORD   ");
          }
      
          input_password = ""; // clear input password
        } 
    
        else 
        {
          input_password += key; // append new character to input password string
          lcd.print("*");
        }
     }
  }



  while (position == 0)
  {
    int a;
    char key = keypad.getKey();

    if (key)
    {
 
        if(key == '*') // clear input password
        {
          input_password = "";
          lockPosition(false);
        } 
 
        else if(key == '#') // enter input password
        {
          if(password == input_password) 
          {
            lockPosition(true);
            position=1;     
          } 
      
          else 
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("WRONG PASSWORD   ");
          }
      
          input_password = ""; // clear input password
        } 
    
        else 
        {
          input_password += key; // append new character to input password string
          lcd.print("*");
        }
     }
  }
}

void lockPosition(int doorLock)
{
  if (doorLock)
  {
    doorMotor.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOR IS LOCKED");
    lcd.setCursor(0,1);
    lcd.print("KEY: ");
  }
  
  else
  {
    doorMotor.write(180);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOR IS UNLOCKED");
    lcd.setCursor(0,1);
    lcd.print("KEY: ");
  }
}
