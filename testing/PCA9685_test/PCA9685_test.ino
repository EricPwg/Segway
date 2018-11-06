#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");
 
  pwm1.begin();
  pwm1.setPWMFreq(1600);

}

void loop() {
  // put your main code here, to run repeatedly:

}
