#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "maxon.h"



//1: pmw on pin 45,44
//2: enable on pin 47,46
//3: direction on pin 49,48


int speed = 200;  //speed in pmw 10%-90% of 0-255
int speedTurn = 80;

float readSpeed;
int commFreq;



void setup() {
  // sets the pin as output
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
  pinMode(PWM_PIN3, OUTPUT);
  pinMode(PWM_PIN4, OUTPUT);
  
  pinMode(EN_PIN1, OUTPUT);
  pinMode(EN_PIN2, OUTPUT);
  pinMode(EN_PIN3, OUTPUT);
  pinMode(EN_PIN4, OUTPUT);
  
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
  pinMode(DIR_PIN3, OUTPUT);
  pinMode(DIR_PIN4, OUTPUT);

  // SERIAL.begin(9600); // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  Serial.println("Robottle ready to take off");
  Serial.println("Waiting for command");


  // default pin setup
  digitalWrite(EN_PIN1, LOW);
  digitalWrite(EN_PIN2, LOW);
  digitalWrite(EN_PIN3, LOW);
  digitalWrite(EN_PIN4, LOW);

  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, LOW);
  digitalWrite(DIR_PIN3, HIGH);
  digitalWrite(DIR_PIN4, LOW);
}

void loop() {
  if(Serial.available()){
    char val = Serial.read();
    Serial.print(val) ; 
    if(val != -1)
    {
      switch(val)
      {
      case 'w'://Move Forward
        moveForward (speed,speed);   //move forward in max speed
        break;
      case 's'://Move Backward
        moveBackward (speed,speed);   //move back in max speed
        break;
      case 'a'://Turn Left
        moveLeft (speedTurn,speedTurn);
        break;
      case 'd'://Turn Right
        moveRight (speedTurn,speedTurn);
        break;
      case 'x':
        stop();
        break;
      // case 'r':
      //   readSpeed = (int)readSpeedL();
      //   if(readSpeed!=1) {
      //     SERIAL.print("speed left:");
      //     SERIAL.println(readSpeed);
      //   }
      //   break;
      // case 't':
      //   readSpeed = (int)readSpeedR();
      //   if(readSpeed!=1) {
      //     SERIAL.print("speed right:");
      //     SERIAL.println(readSpeed);
      //   }
      //   break;
      case 'z':
        Serial.print("test hello");
        Serial.println(123);
      }
    }
    else stop();
  }

  // moveForward (speed,speed);
  // delay(2000);
  // moveBackward(speed, speed);
  // delay(2000);
  // moveLeft(speed,speed);
  // delay(2000);
  // moveRight(speed, speed);
  // delay(2000);

  // readSpeed = readSpeedL();
  // SERIAL.print("read speed LEFT:");
  // SERIAL.println(readSpeed);
  // readSpeed = readSpeedR();
  // SERIAL.print("read speed RIGHT:");
  // SERIAL.println(readSpeed);
  // delay(200);
}
