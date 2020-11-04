#ifndef MAXON_h
#define MAXON_h

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define SERIAL    Serial2

// PINS FOR CONTROL
//FRONT MOTORS: 1 for LEFT, 2 for RIGHT
#define PWM_PIN1        44
#define PWM_PIN2        45
#define EN_PIN1         46
#define EN_PIN2         47
#define DIR_PIN1        48
#define DIR_PIN2        49

//BACKMOTORS: 3 for LEFT, 4 for RIGHT
#define PWM_PIN3        3
#define PWM_PIN4        2
#define EN_PIN3         A5
#define EN_PIN4         A7
#define DIR_PIN3        A4
#define DIR_PIN4        A6


// constants
#define V_ZERO          414   //analog output for zero speed
#define MAX_RPM         3500  //range for rpm measurement
#define MAX_V           409   //limit voltage for max speed (409 ~= 4V)
#define GEAR_RATIO      60
#define PMW_LOW_SPEED   26    //low PMW value for stopping motors
#define ZERO_SPEED      2     //speed low enough for turning off motors
#define SPEED_COMPARE   4     //speed difference for comparing

// functions
//MOVE
void moveForward(int speedL,int speedR);
void moveBackward(int speedL,int speedR);
void moveLeft(int speedL,int speedR);
void moveRight(int speedL,int speedR);
void stop();

// //READ SPEED
// float readSpeedL();
// float readSpeedR();
// float convertVoltToSpeed(float v);



#endif
