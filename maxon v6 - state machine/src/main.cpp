#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "maxon.h"
#include "state_machine.h"


// STATE MACHINE:
// CONTROL_MODE   = receive commands from jetson and send speed
// ROTATION_MODE  = rotation of 360° for scanning surroundings
int state = CONTROL_MODE;


// communication frequency
int commFreq = 9600;

// variables
float readSpeed;
float speedLeft;
float speedRight;


void setup() {
  // sets the pin as output/input
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

  // begin communication
  SERIAL.begin(commFreq);
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
  switch (state)
  {
  case CONTROL_MODE:
    state = controlMode(state);
    break;

  case ROTATION_MODE:
    state = rotationMode(state);
    break; 
  
  default:
    break;
  }

  delay(50);
}


