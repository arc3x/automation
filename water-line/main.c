#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

unsigned long timer = millis();
unsigned long timeon = 900000;   //open time in millis
//15 min = 900000
//= (gal desired/gpm)*60*1000
unsigned long howoften = 86400000;    //how often to water in millis
//86400000 = 1 day 
//= how often in days * 24*60*60*1000

void setup() {
  AFMS.begin();
   myMotor->setSpeed(1);
  Serial.begin(9600);

}

void loop() {
  Serial.println("millis(), timer, howoften, millis-timer :");Serial.print(millis());Serial.print(" | ");Serial.print(timer);Serial.print(" | ");Serial.print(howoften);Serial.print(" | ");Serial.println(millis() - timer);
  if (millis() - timer > howoften) { //test if time to water
    Serial.print(millis());Serial.print("  ");
    Serial.println(millis()-timer);
    Serial.print("timer= ");Serial.println(timer);
    
    myMotor->step(180, FORWARD, INTERLEAVE); 
    Serial.println("on");
    delay(timeon);
    myMotor->step(180, BACKWARD, INTERLEAVE);
    Serial.println("off");
    timer = millis();
    
    
  }
}
