#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

#include "maxon.h"
#include "state_machine.h"
#include "AX12A.h"
#include "servo.h"
#include "Ultrasonic.h"



// state machine
// initial state: control mode
int state = CONTROL_MODE;



void setup() {
// begin communication with arduino
  JETSON_SERIAL.begin(ARDUINO_BD);

  // start servo communications
  // set last arg to false when using JETSON to hide error messages
  setupServo(SERVO_ID, SERVO_BD, SERVO_DIR_PIN, &SERVO_SERIAL, false);
  setupServo(SERVO_DOOR_ID, SERVO_BD, SERVO_DIR_PIN, &SERVO_SERIAL, false);

  // setup maxon pins
  setupMaxon();
}


void loop() {
  switch (state)
  {
  case CONTROL_MODE:
    state = controlMode(state);
    break;

  case ROTATION_MODE:
    state = rotationMode(state);
    break; 
  
  case SETUP_ARM_MODE:
    state = setupArmMode(state);
    break;

  case BOTTLE_PICKING_MODE:
    state = bottlePickingMode(state);
    break;

  case BOTTLE_PICKING2_MODE:
    state = bottlePicking2Mode(state);
    break;
  
  case BOTTLE_REACHING_MODE:
    state = bottleReachingMode(state);
    break;
  
  case RELEASE_MODE:
    state = releaseMode(state);
    break;

  case OFF_MODE:
    state = offMode(state);
    break;
  
  default:
    break;
  }

  delay(50);
}


