/*
PI Controlled 3 wheeled robot
Author: Levi J. Vande Kerkhoff
Date: 10/24/2023
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// ---- pin assignments ----
const int IR_Value = A0; // the infrared sensor input
const int PWM_Drive = 11; // the motor controller output

// ---- variables ----
const float Proportional_Gain = 5; // proportional gain
const float Integral_Gain = 0.1; // integral gain
const int Desired_Distance = 220; // distance from robot to object

volatile int Error = 0; // intial error
volatile int Integral_Error = 0; 
volatile int PWM_Val; // pulse width modulation time length
volatile int i = 0; // interator for LCD screen update delay


// ---- set up ----
void setup() {
  pinMode(IR_Value, INPUT); // infrared sensor signal
  pinMode(PWM_Drive, OUTPUT); // motor drive output

  lcd.init(); // initialize the lcd 
  lcd.backlight(); // turn on the backlight
  lcd.setCursor(0, 0); // set the cursor to the first entry

  Serial.begin(9600); // start serial communication at 9600 bits per second
}

// ---- program loop ----
void loop() {
  Error = Desired_Distance - analogRead(IR_Value); // this finds the error between the current output and desired output
  Integral_Error += Error; // this integrates the error over time
  PWM_Val = 1500 + Error*Proportional_Gain; + Integral_Error*Integral_Gain; // this is the pulse width modulation signal for controlling the motor. This determines how long the signal is high.

  // the following conditional statements restrict the PWM signal to a minimum of 1000 and a maximum of 2000 microseconds 
  if(PWM_Val > 2000){
    PWM_Val = 2000;
  }
  else if(PWM_Val < 1000){
    PWM_Val = 1000;
  }


  if(i >= 200){ // printing to the LCD Screen every 200 iterations of the main loop
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error:");
    lcd.print(Error);
    lcd.setCursor(0,1);
    lcd.print("G1:");
    lcd.print(Proportional_Gain);
    lcd.print("G2:");
    lcd.print(Integral_Gain);
    i = 0;
  }
  i++;
  
  digitalWrite(PWM_Drive, HIGH); // starts the high interval
  delayMicroseconds(PWM_Val); // delays for the high interval length
  digitalWrite(PWM_Drive, LOW); // starts the low interval
  delayMicroseconds(2000 - PWM_Val); // this is the remainder of the total signal time
}
