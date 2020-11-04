#include "maxon.h"


void moveForward(int speedL,int speedR) {
  //if its not going straight, stop first
  if(abs(readSpeedL()-readSpeedR()) > SPEED_COMPARE) {
    stop();
  }
  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, LOW);
  analogWrite(PWM_PIN1, speedL);
  analogWrite(PWM_PIN2, speedR);
}

void moveBackward(int speedL,int speedR) {
  //if its not going straight, stop first
  if(abs(readSpeedL()-readSpeedR()) > SPEED_COMPARE) {
    stop();
  }
  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  digitalWrite(DIR_PIN1, LOW);
  digitalWrite(DIR_PIN2, HIGH);
  analogWrite(PWM_PIN1, speedL);
  analogWrite(PWM_PIN2, speedR);
}

void moveLeft(int speedL,int speedR) {
  //if its going straight, stop first
  if(abs(readSpeedL()+readSpeedR()) > SPEED_COMPARE) {
    stop();
  }
  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  digitalWrite(DIR_PIN1, LOW);
  digitalWrite(DIR_PIN2, LOW);
  analogWrite(PWM_PIN1, speedL);
  analogWrite(PWM_PIN2, speedR);
}

void moveRight(int speedL,int speedR) {
  //if its going straight, stop first
  if(abs(readSpeedL()+readSpeedR()) > SPEED_COMPARE) {
    stop();
  }
  digitalWrite(EN_PIN1, HIGH);
  digitalWrite(EN_PIN2, HIGH);
  digitalWrite(DIR_PIN1, HIGH);
  digitalWrite(DIR_PIN2, HIGH);
  analogWrite(PWM_PIN1, speedL);
  analogWrite(PWM_PIN2, speedR);
}

void stop() {
  analogWrite(PWM_PIN1, PMW_LOW_SPEED);
  analogWrite(PWM_PIN2, PMW_LOW_SPEED);

  //wait until it slows down to almost zero speed or if there is another command
  while(!Serial.available() && (abs(readSpeedL()) >= ZERO_SPEED && abs(readSpeedR()) >= ZERO_SPEED)) {
    delay(20);
  }

  //disable motors
  if(abs(readSpeedL()) <= (ZERO_SPEED+2) && abs(readSpeedR()) <= (ZERO_SPEED+2)) {
    digitalWrite(EN_PIN1, LOW);
    digitalWrite(EN_PIN2, LOW);
  }
}

float readSpeedRawL() {
  //read the analog pin without post processing: used to find V_ZERO
  return analogRead(SPEED_OUT1);
}

// float readSpeedL() {
//   return convertVoltToSpeed(analogRead(SPEED_OUT1));
// }

float readSpeedL() {
  return convertVoltToSpeed(analogRead(SPEEDAVG_OUT1));
}

float readSpeedR() {
  return -convertVoltToSpeed(analogRead(SPEEDAVG_OUT2));
}

float convertVoltToSpeed(float v) {
  return ((v-V_ZERO)*MAX_RPM/MAX_V)/GEAR_RATIO;
}
