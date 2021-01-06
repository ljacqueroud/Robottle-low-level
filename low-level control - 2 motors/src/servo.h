
#ifndef SERVO_h
#define SERVO_h

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// SERVO PARAMETERS

#define SERVO_SERIAL                Serial1
#define SERVO_DIR_PIN             	18
#define SERVO_BD       		          200000
#define SERVO_ID 						        11
#define SERVO_DOOR_ID               2
#define SERVO_MOV_SPEED             200
#define SERVO_MOV_HIGH_SPEED        400


// CONSTANTS

#define TORQUE_LIMIT_SETUP          300         // torque limit for measuring the pickup and release position
#define TORQUE_LIMIT_PICKUP         400         // torque limit when picking up a laying bottle
#define TORQUE_LIMIT_STANDING_PIKUP 800
#define TORQUE_LIMIT_RELEASE        1000        // torque limit when releasing a bottle
#define TORQUE_LIMIT_DOOR           400         // torque limit when closing the back door
#define TORQUE_LIMIT_LOW            180         // low torque limit
#define TORQUE_LIMIT_HIGH           1000        // high torque limit
#define TORQUE_LIMIT_COUNT          50          // limit for iterations to check if arm stuck

#define POS_EPSILON                 5           // tolerance for moving to a target position
#define POS_PICKUP                  50          // position used to calibrate pickup position
#define POS_RELEASE                 1000        // position used to calibrate release position
#define POS_SETUP_SHIFT             20          // shift of resulting positions when calibrating
#define POS_MID                     700         // middle position for calibration
#define POS_STANDING_BOTTLE         190         // position of standing bottle
#define POS_PICKUP_TOL              5           // tolerance to check if a bottle was there
#define POS_CHANGE_COUNT            50          // iteration for not changing position

#define PICK_UP_TRIALS              3           // number of times it can try to pick up a bottle


// FUNCTIONS

void resetAll(unsigned char ID, long bd, unsigned char DirectionPin, HardwareSerial *srl);
void resetID(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl);
void resetBD(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl);
void setupServo(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl, bool showError);

int moveTo(int ID, int16_t target, uint16_t tor_lim);
void moveToNoTorque(int ID, int16_t target);

void setupPos(int ID);
int movePickup(int ID);
int moveRelease(int ID);
void openDoor (int ID);
void closeDoor (int ID);



#endif
