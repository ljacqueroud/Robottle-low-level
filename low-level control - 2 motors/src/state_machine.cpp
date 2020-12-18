#include "state_machine.h"
#include "maxon.h"
#include "servo.h"
#include "Ultrasonic.h"


char val;

long speed = SPEED_TRAVEL;
long speedTurn = SPEED_TURN_TRAVEL;

Ultrasonic ultrasonic(ULTRASONIC_PIN);


///////////////////////////////////////////////// STATE MACHINE ///////////////////////////////////////////////

///////////////////////////////////////////////// CONTROL ///////////////////////////////////////////////
int controlMode (int state) {
  /*
  reads the commands and sends speed to SERIAL 
  */

  // reads commands
  if(JETSON_SERIAL.available()){
    val = JETSON_SERIAL.read();
    
    if(val != -1)
    {
      Serial.println(val);
      switch(val)
      {
      case 'w':     //Move Forward
        moveForward (speed);
        break;
      case 's':     //Move Backward
        moveBackward (speed);
        break;
      case 'a':     //Turn Left
        moveLeft (speedTurn);
        break;
      case 'd':     //Turn Right
        moveRight (speedTurn);
        break;
      case 'x':
        stop();
        break;
      
      case 'e':
        changeSpeeds(JETSON_SERIAL.read());       // change speeds
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

  // readSpeed = readSpeedL();
  // JETSON_SERIAL.print("l");
  // JETSON_SERIAL.println(readSpeed);
  
  // readSpeed = readSpeedR();
  // JETSON_SERIAL.print("r");
  // JETSON_SERIAL.println(readSpeed);

  return state;
  
}


///////////////////////////////////////////////// ROTATION ///////////////////////////////////////////////

int rotationMode (int state) {
  /*
  rotation of 360°, then returns to state CONTROL_MODE
  */

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  moveRight(speedTurn);
  delay(ROT_CONST / (speedTurn - PMW_LOW_SPEED) * ROT_CONST);
  stop();

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_SUCCEDED);

  return CONTROL_MODE;
}


///////////////////////////////////////////////// SETUP ARM ///////////////////////////////////////////////

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


///////////////////////////////////////////////// ARM PICK BOTTLE ///////////////////////////////////////////////

int armPickBottleMode (int state) {
  /*
  pick up a bottle and release inside container
  */

  int bottle_found;

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


///////////////////////////////////////////////// BOTTLE PICKING ///////////////////////////////////////////////

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
  moveForward(speed);

  while(measurement_continuity < ULTRASONIC_BOTTLE_DETECTION_CONTINUITY && iter < MAX_ITER) {
    iter += 1;
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
    delay(2000);
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_SUCCEDED);
    return CONTROL_MODE;
    // return ARM_PICK_BOTTLE_MODE;
  }
  else {
    // something went wrong, no bottle was detected
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_FAILED);

    return CONTROL_MODE;
  }
}


///////////////////////////////////////////////// OFF ///////////////////////////////////////////////

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


////////////////////////////////// utils ////////////////////////////

void changeSpeeds (char c) {
  /*
  change speeds depending on MODE:
  1: travel mode
  2: random search mode
  */

  int iter = 0;

  while(!JETSON_SERIAL.available() && iter < MAX_ITER_SPEED_CHANGE) {       // wait to make sure another char is there
    iter += 1;
    delay(10);
  }
  
  if(iter < MAX_ITER_SPEED_CHANGE) {
    switch(JETSON_SERIAL.read())
    {
    case '1':
      speed = SPEED_TRAVEL;
      speedTurn = SPEED_TURN_TRAVEL;
      break;
    
    case '2':
      speed = SPEED_RANDOM_SEARCH;
      speedTurn = SPEED_TURN_RANDOM_SEARCH;
      break;

    default:
      break;
    }
  }
}