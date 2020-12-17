#include "state_machine.h"
#include "maxon.h"
#include "servo.h"
#include "Ultrasonic.h"


char val;
Ultrasonic ultrasonic(ULTRASONIC_PIN);


int controlMode (int state) {
  /*
  reads the commands and sends speed to SERIAL 
  */

  // reads commands
  if(JETSON_SERIAL.available()){
    val = JETSON_SERIAL.read();
    
    if(val != -1)
    {
      switch(val)
      {
      case 'w':     //Move Forward
        moveForward ();
        break;
      case 's':     //Move Backward
        moveBackward ();
        break;
      case 'a':     //Turn Left
        moveLeft ();
        break;
      case 'd':     //Turn Right
        moveRight ();
        break;
      case 'x':
        stop();
        break;

      case 'r':
        stop();
        state = ROTATION_MODE;
        break;

      case 'o':
        stop();
        state = SETUP_ARM_MODE;
        break;

      case 'p':
        stop();
        state = ARM_PICK_BOTTLE_MODE;
        break;

      case 'y':
        stop();
        state = BOTTLE_PICKING_MODE;
        break;

      case 'v':
        stop();
        state = OFF_MODE;
        break;
      }
    }
    else stop();
  }

  // reads speeds
  int readSpeed;

  readSpeed = readSpeedL();
  JETSON_SERIAL.print("l");
  JETSON_SERIAL.println(readSpeed);
  
  readSpeed = readSpeedR();
  JETSON_SERIAL.print("r");
  JETSON_SERIAL.println(readSpeed);

  return state;
  
}



int rotationMode (int state) {
  /*
  rotation of 360°, then returns to state CONTROL_MODE
  */

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  moveRight();
  delay(ROT_CONST / (SPEED_TURN - PMW_LOW_SPEED) * ROT_CONST);
  stop();

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_SUCCEDED);

  return CONTROL_MODE;
}


int setupArmMode (int state) {
  /*
  move arm down and up to measure pickup and release positions based on torque
  */

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);
  
  setupPos(SERVO_ID);

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_SUCCEDED);

  return CONTROL_MODE;
}


int armPickBottleMode (int state) {
  /*
  pick up a bottle and release inside container
  */

  int bottle_found;

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  bottle_found = movePickup(SERVO_ID);
  delay(100);
  moveRelease(SERVO_ID);

  // send message to jetson
  // if(bottle_found) {
  //   JETSON_SERIAL.print("s");
  //   JETSON_SERIAL.println(TASK_SUCCEDED);
  // }
  // else {
  //   JETSON_SERIAL.print("s");
  //   JETSON_SERIAL.println(TASK_FAILED);
  // }
  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_SUCCEDED);

  return CONTROL_MODE;
}

int bottlePickingMode (int state) {
  /*
  advance until a bottle is detected with ultrasonic sensor
  */

  long measurement = 1000;
  long measurement_continuity = 0;
  long iter = 0;

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  // start going forward
  moveForward();

  while(measurement_continuity < ULTRASONIC_BOTTLE_DETECTION_CONTINUITY && iter < MAX_ITER) {
    // iter += 1;
    measurement = ultrasonic.MeasureInCentimeters();

    if(measurement < ULTRASONIC_BOTTLE_DETECTION) {
      measurement_continuity += 1;
    }
    else {
      measurement_continuity = 0;
    }

    Serial.print("measurement: ");
    Serial.println(measurement);

    delay(50);
  }
 
  // stop when bottle is detected
  stop();
  
  if(iter < MAX_ITER) {
    // bottle detected, use arm to pick up bottle
    return ARM_PICK_BOTTLE_MODE;
  }
  else {
    // something went wrong, no bottle was detected
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_FAILED);

    return CONTROL_MODE;
  }
}


int offMode (int state) {
  /*
  robot is off, cannot do anything
  */

  // reads commands
  if(JETSON_SERIAL.available()){
    val = JETSON_SERIAL.read();
    
    if(val != -1)
    {
      switch(val)
      {
      case 'c':     //Move Forward
        return CONTROL_MODE;
      }
    }
    else stop();
  }

  return OFF_MODE;
}