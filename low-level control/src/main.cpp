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



// state machine
// initial state: control mode
int state = CONTROL_MODE;



void setup() {
// begin communication with arduino
  JETSON_SERIAL.begin(ARDUINO_BD);

  // start servo communication
  // set to last arg to false when using JETSON to hide error messages
  setupServo(SERVO_ID, SERVO_BD, SERVO_DIR_PIN, &SERVO_SERIAL, true);

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

  case PICK_BOTTLE_MODE:
    state = pickBottleMode(state);
    break;
  
  default:
    break;
  }

  delay(50);
}

