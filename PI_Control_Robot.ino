/*
PI Controlled 3 wheeled robot
Author: Levi J. Vande Kerkhoff
Date: 10/24/2023
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// ---- pin assignments ----
const int sense = A0; // the infrared sensor input
const int pwmDrive = 11; // the motor controller output

// ---- variables ----
const float gain1 = 5; // proportional gain
const float gain2 = 0.1; // integral gain
const int input = 220; // distance from robot to object

volatile int error = 0; // intial error
volatile int integral_error = 0; 
volatile int pwmVal; // pulse width modulation time length
volatile int i = 0; // interator for LCD screen update delay


// ---- set up ----
void setup() {
  pinMode(sense, INPUT); // infrared sensor signal
  pinMode(pwmDrive, OUTPUT); // motor drive output

  lcd.init(); // initialize the lcd 
  lcd.backlight(); // turn on the backlight
  lcd.setCursor(0, 0); // set the cursor to the first entry

  Serial.begin(9600);
}

// ---- program loop ----
void loop() {
  error = input - analogRead(sense); // this finds the error between the current output and desired output
  integral_error += error; // this integrates the error over time
  pwmVal = 1500 + error*gain1; + integral_error*gain2;

  // the following conditional statements restrict the PWM signal to a minimum of 1000 and a maximum of 2000 microseconds 
  if(pwmVal > 2000){
    pwmVal = 2000;
  }
  else if(pwmVal < 1000){
    pwmVal = 1000;
  }


  if(i >= 200){ // printing to the LCD Screen every 200 iterations of the main loop
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("error:");
    lcd.print(error);
    lcd.setCursor(0,1);
    lcd.print("g1:");
    lcd.print(gain1);
    lcd.print("g2:");
    lcd.print(gain2);
    i = 0;
  }
  i++;
  
  digitalWrite(pwmDrive, HIGH); // starts the high interval
  delayMicroseconds(pwmVal); // delays for the high interval length
  digitalWrite(pwmDrive, LOW); // starts the low interval
  delayMicroseconds(2000 - pwmVal); // this is the remainder of the total signal time
}
