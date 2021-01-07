#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// states
#define CONTROL_MODE            0
#define ROTATION_MODE           1
#define SETUP_ARM_MODE          2
#define BOTTLE_PICKING_MODE     3
#define BOTTLE_PICKING2_MODE    4
#define BOTTLE_REACHING_MODE    5
#define ROCKS_REACHING_MODE     11
#define RELEASE_MODE            6
#define RECOVERY_MODE           7
#define RECOVERY2_MODE          8
#define ROCKS_MODE              9
#define OFF_MODE                10

// messages to send
#define TASK_FAILED             0
#define TASK_SUCCEDED           1
#define TASK_IN_PROGRESS        2
#define TASK_ROCKS              3
#define TASK_ROCKS_FINISHED     4

#define MAX_ITER_SPEED_CHANGE   100   // maximum of iteration when waiting for second char of change speed command 'cX'


// functions
int controlMode (int state);
int rotationMode (int state);
int setupArmMode (int state);
int bottlePickingMode (int state);
int bottlePicking2Mode (int state);
int bottleReachingMode(int state);
int rocksReachingMode (int state);
int releaseMode(int state);
int recoveryMode (int state);
int recovery2Mode(int state);
int rocksMode (int state);
int offMode (int state);

void changeSpeeds ();
int checkBottleInFront();
int checkBottleCentered();
int checkBottleSide();
void alignWithBottle (int sensor_number);



#endif