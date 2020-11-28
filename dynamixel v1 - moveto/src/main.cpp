/*
 * Example of a simple Blink using the AX-12A built-in LED
 */

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

#include "AX12A.h"
#include "servo.h"

//#include <iostream>
// using namespace std;

#define DirectionPin 	(18u)
#define BaudRate  		(200000ul)
#define ID 						(11u)


// /*
// pos: 0=min, 1023=max for range of 300° (10°=34.1)
int posRest = 750;
int posGrab = 130;

// speed: 1=min, 1023=max
uint16_t movSpeed = 100;

// variables
int16_t currentPos;
int errorByte;


void setup()
{
  Serial.begin(9600);

  //begin com
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	delay(50);

  // set limit min angle to 0
  errorByte = ax12a.writeRegister2(ID, AX_CW_ANGLE_LIMIT_L, 0x0000);     //set max angle
  if(errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER CW ANGLE LIMIT ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }

  // set limit max angle to 0x03ff
  errorByte = ax12a.writeRegister2(ID, AX_CCW_ANGLE_LIMIT_L, 0x03FF);     //set max angle
  if(errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER CCW ANGLE LIMIT ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }
  
  // set speed of turning
  errorByte = ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, movSpeed);     //set speed
  if(errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER GOAL SPEED ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }

  delay(50);

  Serial.println("setup finished");
}


void loop()
{
  if(Serial.available()) {
    char val = Serial.read();

    if(val != -1) {
      switch(val) {
      case 's':
        setupPos(ID);
        delay(100);
        break;
      
      case 'p':
        Serial.println(movePickup(ID));
        delay(100);
        moveRelease(ID);
        break;
      
      default:
        break;

      }
    }
  }

  delay(50);
}


