/*
 * Water Line automation
 * 
 * 
 */

#include <stdarg.h>

/* 
    This code should be pasted within the files where this function is needed.
    This function will not create any code conflicts.
    
    The function call is similar to printf: ardprintf("Test %d %s", 25, "string");

    To print the '%' character, use '%%'

    This code was first posted on http://arduino.stackexchange.com/a/201
*/

#ifndef ARDPRINTF
#define ARDPRINTF
#define ARDBUFFER 16 //Buffer for storing intermediate strings. Performance may vary depending on size.
#include <stdarg.h>
#include <Arduino.h> //To allow function to run from any file in a project

int ardprintf(char *str, ...) //Variadic Function
{
  int i, count=0, j=0, flag=0;
  char temp[ARDBUFFER+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  count++; //Evaluate number of arguments required to be printed
  
  va_list argv;
  va_start(argv, count);
  for(i=0,j=0; str[i]!='\0';i++) //Iterate over formatting string
  {
    if(str[i]=='%')
    {
      //Clear buffer
      temp[j] = '\0'; 
      Serial.print(temp);
      j=0;
      temp[0] = '\0';
      
      //Process argument
      switch(str[++i])
      {
        case 'd': Serial.print(va_arg(argv, int));
                  break;
        case 'l': Serial.print(va_arg(argv, long));
                  break;
        case 'u': Serial.print(va_arg(argv, unsigned long));
                  break;
        case 'f': Serial.print(va_arg(argv, double));
                  break;
        case 'c': Serial.print((char)va_arg(argv, int));
                  break;
        case 's': Serial.print(va_arg(argv, char *));
                  break;
        default:  ;
      };
    }
    else 
    {
      //Add to buffer
      temp[j] = str[i];
      j = (j+1)%ARDBUFFER;
      if(j==0)  //If buffer is full, empty buffer.
      {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  
  Serial.println(); //Print trailing newline
  return count + 1; //Return number of arguments detected
}

#undef ARDBUFFER
#endif

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
const unsigned long SEC_5 = 5000;
const unsigned long SEC_1 = 1000;

// set pin numbers
const int buttonPin = 12;    
const int howoftenPot = 0;
const int timeonPot = 1; 
const int ledPin =  13;      

// stepper motor
Adafruit_MotorShield AFMS; 
Adafruit_StepperMotor* myMotor;

// timer variables
unsigned long timer, timeon, howoften;

// button variables
int buttonRead, buttonState;
unsigned long buttonTimer;

// application variables
bool skipNextWatering = false;
bool waterNow = false;
bool motorInUse = false;

void setup() {
  // enable serial monitor
  Serial.begin(9600);
  // init the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // init the buttonPin as an input:
  pinMode(buttonPin, INPUT);
  // init button
  buttonState = 0; 
  buttonRead = 0;
  buttonTimer = 0;
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
  howoften = MIN_1;    // watering interval
  timeon = SEC_10;     // time valve remains open  

  // startup delay to allow user input to settle
  while (millis() - timer < SEC_10) {
    idle();
  }
}

void loop() {  
  if (!skipNextWatering) {
    // open valve & delay
    moveMotorThenDelay(FORWARD, timeon);
    // close valve
    moveMotorThenDelay(BACKWARD, 0);
    // reset timer
        
  } 
  timer = millis() - timeon;
  skipNextWatering = false;
  // idle for the delay period
  while ((millis() - timer < howoften) && idle()) {
    if (waterNow == true) {
      waterNow = false;
      break;
    }
  }

}

// idle loop returns 0 - kill idle
//                   1 - standard success

bool idle() {
  // default return success
  //int returnValue = 1;
  // debug    
  ardprintf("clock: %u / %u\tbuttonState: %d\tskipNextWatering: %d\thowoftenPot: %d\n", millis()-timer, howoften, buttonState, skipNextWatering, analogRead(howoftenPot));
  // read sensors
  buttonRead = digitalRead(buttonPin);

  // do logic
  if (buttonRead == 1) {    
    if (skipNextWatering == true) {
      skipNextWatering = false;            
      buttonRead = digitalRead(buttonPin);
      delay(1000);
      timer += 500;
    } else {
      skipNextWatering = true;
      delay(5000);    
      timer += 5000;
      buttonRead = digitalRead(buttonPin);
      //ardprintf("timer: %u\tclock: %u / %u\tbuttonState: %d\tskipNextWatering: %d\n", timer, millis()-timer, howoften, buttonState, skipNextWatering);
      if (buttonRead == 1) {
        skipNextWatering = false;
        waterNow = true;
        //returnValue = 1;
      }
    }
    
  }

  return true;
  //return returnValue;
}

// mover the motor forward and delay for delayAfter milliseconds after move
void moveMotorThenDelay(int direction, unsigned long delayAfter) {
  motorInUse = true;
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
  motorInUse = false;
}


