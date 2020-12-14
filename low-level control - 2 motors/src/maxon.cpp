#include "maxon.h"


// SETUP

void setupMaxon() {
  // sets the pin as output/input
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
  // pinMode(PWM_PIN3, OUTPUT);
  // pinMode(PWM_PIN4, OUTPUT);
  
  pinMode(EN_PIN1, OUTPUT);
  pinMode(EN_PIN2, OUTPUT);
  // pinMode(EN_PIN3, OUTPUT);
  // pinMode(EN_PIN4, OUTPUT);
  
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
  // pinMode(DIR_PIN3, OUTPUT);
  // pinMode(DIR_PIN4, OUTPUT);

  pinMode(DIGITAL_OUT1, INPUT);
  pinMode(DIGITAL_OUT2, INPUT);
  pinMode(SPEED_OUT1, INPUT);
  pinMode(SPEED_OUT2, INPUT);
  pinMode(SPEEDAVG_OUT1, INPUT);
  pinMode(SPEEDAVG_OUT2, INPUT);

  // default pin setup
  digitalWrite(EN_PIN1, LOW);
  digitalWrite(EN_PIN2, LOW);
  // digitalWrite(EN_PIN3, LOW);
  // digitalWrite(EN_PIN4, LOW);

  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, LOW);
  // digitalWrite(DIR_PIN3, HIGH);
  // digitalWrite(DIR_PIN4, LOW);
}


// MOVEMENT FUNCTIONS

void moveForward() {
  //if its not going straight, stop first
  if(abs(readSpeedL()-readSpeedR()) > SPEED_COMPARE) {
    stop();
  }
  
  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  // digitalWrite(EN_PIN3, HIGH);
  // digitalWrite(EN_PIN4, HIGH);

  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, LOW);
  // digitalWrite(DIR_PIN3, HIGH);
  // digitalWrite(DIR_PIN4, LOW);

  analogWrite(PWM_PIN1, SPEED_STRAIGHT);
  analogWrite(PWM_PIN2, SPEED_STRAIGHT);
  // analogWrite(PWM_PIN3, SPEED_STRAIGHT);
  // analogWrite(PWM_PIN4, SPEED_STRAIGHT);
}

void moveBackward() {
  //if its not going straight, stop first
  if(abs(readSpeedL()-readSpeedR()) > SPEED_COMPARE) {
    stop();
  }

  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  // digitalWrite(EN_PIN3, HIGH);
  // digitalWrite(EN_PIN4, HIGH);

  digitalWrite(DIR_PIN1, LOW);
  digitalWrite(DIR_PIN2, HIGH);
  // digitalWrite(DIR_PIN3, LOW);
  // digitalWrite(DIR_PIN4, HIGH);

  analogWrite(PWM_PIN1, SPEED_STRAIGHT);
  analogWrite(PWM_PIN2, SPEED_STRAIGHT);
  // analogWrite(PWM_PIN3, SPEED_STRAIGHT);
  // analogWrite(PWM_PIN4, SPEED_STRAIGHT);
}

void moveLeft() {
  //if its going straight, stop first
  if(abs(readSpeedL()+readSpeedR()) > SPEED_COMPARE) {
    stop();
  }

  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  // digitalWrite(EN_PIN3, LOW);
  // digitalWrite(EN_PIN4, LOW);

  digitalWrite(DIR_PIN1, LOW);
  digitalWrite(DIR_PIN2, LOW);
  
  analogWrite(PWM_PIN1, SPEED_TURN);
  analogWrite(PWM_PIN2, SPEED_TURN);
}

void moveRight() {
  //if its going straight, stop first
  if(abs(readSpeedL()+readSpeedR()) > SPEED_COMPARE) {
    stop();
  }

  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  // digitalWrite(EN_PIN3, LOW);
  // digitalWrite(EN_PIN4, LOW);
  
  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, HIGH);
  
  analogWrite(PWM_PIN1, SPEED_TURN);
  analogWrite(PWM_PIN2, SPEED_TURN);
}

void stop() {
  analogWrite(PWM_PIN1, PMW_LOW_SPEED);
  analogWrite(PWM_PIN2, PMW_LOW_SPEED);
  // analogWrite(PWM_PIN3, PMW_LOW_SPEED);
  // analogWrite(PWM_PIN4, PMW_LOW_SPEED);

  //wait until it slows down to almost zero speed or if there is another command
  while(!JETSON_SERIAL.available() && (abs(readSpeedL()) >= ZERO_SPEED && abs(readSpeedR()) >= ZERO_SPEED)) {
    delay(20);
  }

  //disable motors
  digitalWrite(EN_PIN1, LOW);
  digitalWrite(EN_PIN2, LOW);
  // digitalWrite(EN_PIN3, LOW);
  // digitalWrite(EN_PIN4, LOW);
}

float readSpeedL() {
  return convertVoltToSpeed(analogRead(SPEEDAVG_OUT1) - V_ZERO_L);
}

float readSpeedR() {
  return -convertVoltToSpeed(analogRead(SPEEDAVG_OUT2) - V_ZERO_R);
}

float convertVoltToSpeed(float v) {
  return (v*MAX_RPM/MAX_V)/GEAR_RATIO;
}

