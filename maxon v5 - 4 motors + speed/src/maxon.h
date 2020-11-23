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
#define PWM_PIN3        11
#define PWM_PIN4        7
#define EN_PIN3         10
#define EN_PIN4         6
#define DIR_PIN3        8
#define DIR_PIN4        5

// PINS FOR OUTPUT
#define DIGITAL_OUT1    A0          // currently not used
#define DIGITAL_OUT2    A4          // currently not used
#define SPEED_OUT1      A1          // currently not used
#define SPEED_OUT2      A5          // currently not used
#define SPEEDAVG_OUT1   A2
#define SPEEDAVG_OUT2   A6

// ////////////////////////////// ATTENTION: DONT CONNECT TO PIN 9 ///////////////////////////////


// constants
#define V_ZERO_L        409   //analog output for zero speed on left driver
#define V_ZERO_R        410   //analog output for zero speed on right driver
#define MAX_RPM         3500  //range for rpm measurement
#define MAX_V           410   //limit voltage for max speed (409 ~= 4V)
#define GEAR_RATIO      60
#define PMW_LOW_SPEED   26    //low PMW value for stopping motors
#define PMW_HIGH_SPEED  229   //high PMW value for maximum speed
#define ZERO_SPEED      2     //speed low enough for turning off motors
#define SPEED_COMPARE   4     //speed difference for comparing

// functions
//MOVE
void moveForward(int speedL,int speedR);
void moveBackward(int speedL,int speedR);
void moveLeft(int speedL,int speedR);
void moveRight(int speedL,int speedR);
void stop();

//READ SPEED
float readSpeedL();
float readSpeedR();
float convertVoltToSpeed(float v);



#endif
