

#include <Wire.h>
#include "Adafruit_MotorShield.h"
#include "Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

unsigned long DAY_1 = 86400000;
unsigned long DAY_2 = 172800000;
unsigned long MIN_15 = 900000;
unsigned long MIN_2 = 120000;   
unsigned long MIN_1 = 60000;



unsigned long timer = millis();
unsigned long timeon = MIN_15;   //open time in millis
unsigned long howoften = DAY_2;    //how often to water in millis


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
    myMotor->release();
    Serial.println("on");
    delay(timeon);
    myMotor->step(180, BACKWARD, INTERLEAVE);
    myMotor->release();
    Serial.println("off");
    timer = millis();
    
    
  }
}



