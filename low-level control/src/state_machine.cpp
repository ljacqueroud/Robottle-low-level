#include "state_machine.h"
#include "maxon.h"
#include "servo.h"


char val;


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
        state = PICK_BOTTLE_MODE;
        break;
      }
    }
    else stop();
  }

  // reads speeds
  int readSpeed;

  // readSpeed = readSpeedL();
  // JETSON_SERIAL.print("l");
  // JETSON_SERIAL.println(readSpeed);
  
  // readSpeed = readSpeedR();
  // JETSON_SERIAL.print("r");
  // JETSON_SERIAL.println(readSpeed);

  return state;
  
}



int rotationMode (int state) {
  /*
  rotation of 360°, then returns to state CONTROL_MODE
  */

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  moveRight();
  delay(SPEED_TURN * ROT_CONST);
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


int pickBottleMode (int state) {
  /*
  pick up a bottle and release inside container
  */

  int bottle_found;

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  bottle_found = movePickup(SERVO_ID);
  delay(100);
  moveRelease(SERVO_ID);

  if(bottle_found) {
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_SUCCEDED);
  }
  else {
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_FAILED);
  }

  return CONTROL_MODE;
}