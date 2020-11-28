
#ifndef SERVO_h
#define SERVO_h

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// CONSTANTS

#define TORQUE_LIMIT_SETUP          300         // torque limit for measuring the pickup and release position
#define TORQUE_LIMIT_PICKUP         400         // torque limit when picking up a bottle
#define TORQUE_LIMIT_RELEASE        600         // torque limit when releasing a bottle
#define TORQUE_LIMIT_LOW            200         // low torque limit
#define TORQUE_LIMIT_HIGH           800         // high torque limit
#define TORQUE_LIMIT_COUNT          50          // limit for iterations to check if arm stuck

#define POS_EPSILON                 5           // tolerance for moving to a target position
#define POS_PICKUP                  50          // position used to calibrate pickup position
#define POS_RELEASE                 1000        // position used to calibrate release position
#define POS_SETUP_SHIFT             20          // shift of resulting positions when calibrating
#define POS_MID                     600         // middle position for calibration
#define POS_PICKUP_TOL              5           // tolerance to check if a bottle was there


// FUNCTIONS

void resetAll(unsigned char ID, long bd, unsigned char DirectionPin, HardwareSerial *srl);
void resetID(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl);
void reserBD(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl);

void moveTo(int id, uint16_t target);
void setupPos(int ID);
int movePickup(int ID);
int moveRelease(int ID);



#endif
