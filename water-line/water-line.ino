/*
 * Water Line automation
 * 
 * 
 */

#include <Wire.h>
#include "Adafruit_MotorShield.h"
#include "Adafruit_MS_PWMServoDriver.h"

// define boolean values because..
typedef int bool;
#define true 1
#define false 0

// debug for testing without motor | false=off - motor present,   true=on - no motor
const bool DEBUG_NO_MOTOR = false;

// time constants
const unsigned long DAY_1 = 86400000;
const unsigned long DAY_2 = 172800000;
const unsigned long MIN_15 = 900000;
const unsigned long MIN_2 = 120000;   
const unsigned long MIN_1 = 60000;
const unsigned long SEC_30 = 30000;
const unsigned long SEC_10 = 10000;

// stepper motor
Adafruit_MotorShield AFMS; 
Adafruit_StepperMotor* myMotor;

// timer variables
unsigned long timer, timeon, howoften;

void setup() {
  // enable serial monitor
  Serial.begin(9600);
  // init motor
  if (!DEBUG_NO_MOTOR) {
    AFMS = Adafruit_MotorShield(); 
    myMotor = AFMS.getStepper(200, 2);
    AFMS.begin();
    myMotor->setSpeed(1); 
  } 
  // init timer
  timer = millis();

  // ***
  // Application variables
  // ***
  howoften = SEC_30;    // watering interval
  timeon = SEC_10;     // time valve remains open  
}

void loop() {
  // open valve & delay
  moveMotorThenDelay(FORWARD, timeon);
  // close valve
  moveMotorThenDelay(BACKWARD, 0);
  // idle for the delay period
  while (millis() - timer < howoften) {
    idle();
  }
  // reset timer
  timer = millis();
}

// idle loop
void idle() {
  // debug
  Serial.println("millis(), timer, howoften, millis-timer :");Serial.print(millis());Serial.print(" | ");Serial.print(timer);Serial.print(" | ");Serial.print(howoften);Serial.print(" | ");Serial.println(millis() - timer);    
}

// mover the motor forward and delay for delayAfter milliseconds after move
void moveMotorThenDelay(int direction, unsigned long delayAfter) {
  direction==FORWARD ? Serial.println("opening valve") : Serial.println("closing valve");
  if (!DEBUG_NO_MOTOR) {
    myMotor->step(180, direction, DOUBLE); 
    myMotor->release();
  } else {
    // TODO: simulate motor
  }
  if (delayAfter > 0) {
    Serial.print("sleeping ");Serial.print((unsigned long)delayAfter/(unsigned long)1000);Serial.println(" seconds");
  }
  delay(delayAfter);  
}


