#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "maxon.h"



//1: pmw on pin 45,44
//2: enable on pin 47,46
//3: direction on pin 49,48


int speed = 130;  //speed in pmw 10%-90% of 0-255
int speedTurn = 80;

float readSpeed;
float speedLeft;
float speedRight;
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

  pinMode(DIGITAL_OUT1, INPUT);
  pinMode(DIGITAL_OUT2, INPUT);
  pinMode(SPEED_OUT1, INPUT);
  pinMode(SPEED_OUT2, INPUT);
  pinMode(SPEEDAVG_OUT1, INPUT);
  pinMode(SPEEDAVG_OUT2, INPUT);

  SERIAL.begin(9600); // opens serial port, sets data rate to 9600 bps
  // Serial.begin(9600);
  // Serial.println("Robottle ready to take off");
  // Serial.println("Waiting for command");


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
  if(SERIAL.available()){
    char val = SERIAL.read();
    // Serial.print(val) ; 
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
      case 'r':
        

      // case 'z':
      //   Serial.print("test hello");
      //   Serial.println(123);
      }
    }
    else stop();
  }

  speedLeft = readSpeedL();
  SERIAL.print("l");
  SERIAL.println(speedLeft);
  // Serial.print("l");
  // Serial.println(speedLeft);
  
  speedRight = readSpeedR();
  SERIAL.print("r");
  SERIAL.println(speedRight);
  // Serial.print("r");
  // Serial.println(speedRight);

  delay(50);
  
}
