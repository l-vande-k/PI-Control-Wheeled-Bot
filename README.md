# PI-Control-Wheeled-Bot
The following repo contains the C++ programming for a PI controlled wheeled robot with an ultrasonic sensor and LCD screen.

The results of the lab can be viewed here:
https://youtu.be/qUs6icUdHRY

Components:
- Serial interface adapter for LCD 1602
  - https://www.amazon.com/dp/B09NMDQVY2?psc=1&ref=ppx_yo2ov_dt_b_product_details
- 1602A LCD Display (16 pin)
- Arduino Uno Rev3
- Arduino Prototyping Shield
- 3-Wire VEX Servo (PWM controlled)
- Aanalog Infrared Sensor (Datasheet below)
  - https://global.sharp/products/device/lineup/data/pdf/datasheet/gp2y0a21yk_e.pdf
- Jumper Wires
- Dupont Wires

Purpose Statment of the Lab:
This lab experiment introduced students to the practicality and versatility of PID controls with a simple PI controlled mobile robot. The program kept the robot at a distance specified by the user from a forward object. It also used the LiquidCrystal_I2C.h library for displaying pertinent information on an LCD display.

Explanation of Project:
The robot in this lab was controlled by a PI controller that used the output of an IR sensor to "see" the object forward of the robot. The IR sensor used to estimate the distance between the robot and the forward object returned an analog voltage value. The value used in the program to represent the desired distance was 220; this corresponds to around 1.08V or 28cm. The PWM signal used to drive the motor is created using the error and integral of the error in the system. The error is found by subtracting the current value returned by the IR sensor from the desired value, 220. This error is summed over each iteration of the main loop to get the integral of the error over time. These can be seen in lines 41 and 42 of the program shown below:

  Error = Desired_Distance - analogRead(IR_Value); // this finds the error between the current output and desired output
  Integral_Error += Error; // this integrates the error over time

Before going any further, we need to understand how PWM signals are used to control the motor. Pulse Width Modulation, PWM, is a digital, square wave signal. This signal is comprised of only two values, low and high. The ratio of the length of the high signal to the length of the period is called the duty cycle. A high signal of 1500 microseconds out of a total 2000 microsecond period creates a duty cycle of 75%. For this motor controller, a 50% duty cycle causes the motor to drive in reverse at full speed while a 100% duty cycle causes the motor to turn forwards at full speed. The value in the middle, a 75% duty cycle, tells the motor to stop turning.

The error and integral error are used to create the PWM control signal sent to the motor. The error and integral error signals are multiplied by a proportional gain and an integral gain, respectively. These signals are used to modify the duty cycle of the PWM control signal to generate an appropriate response to the error in the system. This can be seen in line 43 of the program shown below:

  PWM_Val = 1500 + Error*Proportional_Gain; + Integral_Error*Integral_Gain; // This determines how long the signal is high.

The PWM signal created by the controller is passed through two conditional statements that act as a filter. This filter ensures that the PWM signal length always remains within the desired period length. After this, the signal is sent to the motor and the process repeats.
