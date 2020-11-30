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
#define PICK_BOTTLE_MODE        3

// messages to send
#define TASK_FAILED             0
#define TASK_SUCCEDED           1
#define TASK_IN_PROGRESS        2


// functions
int controlMode (int state);
int rotationMode (int state);
int setupArmMode (int state);
int pickBottleMode (int state);



#endif