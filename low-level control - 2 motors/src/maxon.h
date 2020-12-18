#ifndef MAXON_h
#define MAXON_h

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



// ///////////////////////// CONSTANTS ////////////////////////////////////////////////////


#define JETSON_SERIAL       Serial2
#define ARDUINO_BD          9600


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

// ATTENTION: DONT CONNECT TO PIN 9


// speeds
#define SPEED_TRAVEL              220   //speed in pmw 10%-90% of 0-255 (= 26-229)
#define SPEED_TURN_TRAVEL         80
#define SPEED_RANDOM_SEARCH       140
#define SPEED_TURN_RANDOM_SEARCH  50

#define PMW_LOW_SPEED   26    //lowest speed
#define PMW_HIGH_SPEED  229   //highest speed
#define ZERO_SPEED      2     //speed low enough for turning off motors
#define SPEED_COMPARE   4     //speed difference for comparing

// constants for reading speed
#define V_ZERO_L        409   //analog output for zero speed on left driver
#define V_ZERO_R        410   //analog output for zero speed on right driver
#define MAX_RPM         3500  //range for rpm measurement
#define MAX_V           410   //limit voltage for max speed (409 ~= 4V)
#define GEAR_RATIO      60

// other constants
#define ROT_CONST       680    // constant for completing a 360 rotation (time is squared, divided by speed)
#define MAX_ITER        100    // MAX_ITER*50 = time in ms before quitting BOTTLE_PICKING_MODE in case of no detection


// ////////////////// FUNCTIONS ///////////////////////////////////////////////////////

// SETUP
void setupMaxon();

// MOVE
void moveForward(long speed);
void moveBackward(long speed);
void moveLeft(long speedTurn);
void moveRight(long speedTurn);
void stop();

// READ SPEED
float readSpeedL();
float readSpeedR();
float convertVoltToSpeed(float v);



#endif
