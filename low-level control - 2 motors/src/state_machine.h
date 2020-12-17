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
#define ARM_PICK_BOTTLE_MODE    3
#define BOTTLE_PICKING_MODE     4
#define OFF_MODE                9

// messages to send
#define TASK_FAILED             0
#define TASK_SUCCEDED           1
#define TASK_IN_PROGRESS        2


// functions
int controlMode (int state);
int rotationMode (int state);
int setupArmMode (int state);
int armPickBottleMode (int state);
int bottlePickingMode(int state);
int offMode (int state);



#endif