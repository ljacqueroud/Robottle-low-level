#include "state_machine.h"
#include "maxon.h"
#include "servo.h"
#include "Ultrasonic.h"


char val;

long speed = SPEED_TRAVEL;
long speedTurn = SPEED_TURN_TRAVEL;

Ultrasonic ultrasonic1(ULTRASONIC_PIN1);    // right side
Ultrasonic ultrasonic2(ULTRASONIC_PIN2);    // center
Ultrasonic ultrasonic3(ULTRASONIC_PIN3);    // left side


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
      // Serial.println(val);
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
      
      case 'm':
        changeSpeeds();       // change speeds
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
        state = BOTTLE_PICKING_MODE;
        break;

      case 'P':
        stop();
        state = BOTTLE_PICKING2_MODE;
        break;

      case 'y':
        stop();
        state = BOTTLE_REACHING_MODE;
        break;

      case 'q':
        state = RELEASE_MODE;
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
  // int readSpeed;

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

  
  // rotate 45 degrees
  moveRight(SPEED_ROTATION_MODE);
  delay(ROT_MODE_ROT_TIME/ 6 / (SPEED_ROTATION_MODE - PMW_LOW_SPEED) * ROT_MODE_ROT_TIME);
  stop();

  // move forward a bit
  moveForward(SPEED_RANDOM_SEARCH);
  delay(ROT_MODE_FORWARD_TIME);
  stop();

  // rotate 360 degrees
  moveRight(SPEED_ROTATION_MODE);
  delay(ROT_MODE_ROT_TIME / (SPEED_ROTATION_MODE - PMW_LOW_SPEED) * ROT_MODE_ROT_TIME);
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


///////////////////////////////////////////////// BOTTLE REACHING ///////////////////////////////////////////////

int bottleReachingMode (int state) {
  /*
  advance until a bottle is detected with ultrasonic sensor
  */

  int bottle_in_front = 0;
  long measurement_continuity = 0;
  long iter = 0;

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  // start going forward
  moveForward(SPEED_REACHING_MODE);
  
  while(measurement_continuity < ULTRASONIC_BOTTLE_DETECTION_CONTINUITY && iter < MAX_ITER) {
    // check if there is a message from jetson
    if(JETSON_SERIAL.available()) {
      return CONTROL_MODE;
    }

    iter += 1;
    
    bottle_in_front = checkBottleInFront();   // returns which sensor sensed the bottle

    // check if it detected a bottle
    if(bottle_in_front) {
      measurement_continuity += 1;
    }
    else {
      measurement_continuity = 0;
    }

    delay(ULTRASONIC_MEASURE_DELAY);
  }
 
  // stop when bottle is detected
  stop();

  // align itself with bottle
  alignWithBottle(bottle_in_front);
  
  if(iter < MAX_ITER) {
    // succesfully found a bottle
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_SUCCEDED);
  }
  else {
    // something went wrong, no bottle was detected
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_FAILED);
  }

  return CONTROL_MODE;
}


///////////////////////////////////////////////// BOTTLE PICKING ///////////////////////////////////////////////

int bottlePickingMode (int state) {
  /*
  pick up a bottle and release inside container
  */

  int detect_bottle;

  // move forward a bit
  moveForward(SPEED_REACHING_MODE);
  delay(800);
  stop();

  delay(1000);
  detect_bottle = checkBottleInFront();

  if(!detect_bottle) {
    // move backward a bit
    moveBackward(SPEED_REACHING_MODE);
    delay(800);
    stop();
  }

  for (int i = 0; i < PICK_UP_TRIALS; i++) {
    movePickup(SERVO_ID);
    delay(200);
    moveRelease(SERVO_ID);

    // if there is no bottle, it picked it up
    detect_bottle = checkBottleInFront();

    if (!detect_bottle)
      break;

    delay(200);
  }

  // send message to jetson
  if(!detect_bottle) {
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_SUCCEDED);
  }
  else {
    JETSON_SERIAL.print("s");
    JETSON_SERIAL.println(TASK_FAILED);
  }

  return CONTROL_MODE;
}


///////////////////////////////////////////////// BOTTLE PICKING 2 ///////////////////////////////////////////////

int bottlePicking2Mode (int state) {
  /*
  bottle is standing up: advance, align, pickup
  */

  // move forward a bit
  moveForward(SPEED_REACHING_MODE);
  delay(1000);
  stop();

  // wait a little bit
  delay(1000);

  // detect bottle
  int sensor = checkBottleInFront();

  if(sensor) {
    // align with bottle
    alignWithBottle(sensor);
  }

  // pick up the bottle
  state = bottlePickingMode(BOTTLE_PICKING_MODE);

  return state;
}


///////////////////////////////////////////////// RELEASE ///////////////////////////////////////////////

int releaseMode(int state) {
  /*
  open the back door to release the bottles
  */

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_IN_PROGRESS);

  // rotate of 180 degrees
  moveRight(SPEED_ROTATION_MODE);
  delay(ROT_MODE_ROT_TIME/2 / (SPEED_ROTATION_MODE - PMW_LOW_SPEED) * ROT_MODE_ROT_TIME);
  stop();

  openDoor(SERVO_DOOR_ID);

  // shake a little bit
  for (int i; i<5; i++) {
    moveLeft(SPEED_TURN_RELEASE_MODE);
    delay(100);
    moveRight(SPEED_TURN_RELEASE_MODE);
    delay(100);
  }
  stop();

  closeDoor(SERVO_DOOR_ID);

  JETSON_SERIAL.print("s");
  JETSON_SERIAL.println(TASK_SUCCEDED);

  return CONTROL_MODE;
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

void changeSpeeds () {
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


int checkBottleInFront() {
  /*
  returns 0 if no bottle is detected, 1,2,3 otherwise, depending on the sensor
  */

  long measurement1 = ultrasonic1.MeasureInCentimeters();
  long measurement2 = ultrasonic2.MeasureInCentimeters();
  long measurement3 = ultrasonic3.MeasureInCentimeters();

  // Serial.print("measurements: ");
  // Serial.print(measurement1);
  // Serial.print(" ");
  // Serial.print(measurement2);
  // Serial.print(" ");
  // Serial.println(measurement3);

  if (measurement2 < ULTRASONIC_BOTTLE_DETECTION2) {
    return 2;
  }
  else if (measurement1 < ULTRASONIC_BOTTLE_DETECTION1) {
    return 1;
  }
  else if (measurement3 < ULTRASONIC_BOTTLE_DETECTION1) {
    return 3;
  }

  return 0;
}


int checkBottleCentered() {
  /*
  returns true if bottle is in centered in front of robot
  */

  int bottle_centered;
  long measurement = ultrasonic2.MeasureInCentimeters();

  bottle_centered = measurement < ULTRASONIC_BOTTLE_DETECTION2;

  // Serial.print("measure 2: ");
  // Serial.println(measurement);

  return bottle_centered;
}


int checkBottleSide() {
  /*
  returns 0 if bottle on left side, 1 if bottle on right side, 2 if bottle is neither
  */

  long measurement1 = ultrasonic1.MeasureInCentimeters();
  long measurement3 = ultrasonic3.MeasureInCentimeters();

  
  // Serial.print("measure 1, 3: ");
  // Serial.print(measurement1);
  // Serial.print(" ");
  // Serial.println(measurement3);

  if (measurement1 < ULTRASONIC_BOTTLE_DETECTION1 && measurement3 >= ULTRASONIC_BOTTLE_DETECTION1) {
    return 1;
  }
  else if (measurement1 >= ULTRASONIC_BOTTLE_DETECTION1 && measurement3 < ULTRASONIC_BOTTLE_DETECTION1) {
    return 0;
  }
  else if (measurement1 < ULTRASONIC_BOTTLE_DETECTION1 && measurement3 < ULTRASONIC_BOTTLE_DETECTION1) {
    if (measurement1 < measurement3) {
      return 1;
    }
    else {
      return 0;
    }
  }
  
  return 2;
}


void alignWithBottle (int sensor_number) {
  if (sensor_number == 3) {
    // turn 30 degrees
    moveLeft(SPEED_TURN_REACHING_MODE);
    delay(ROT_MODE_ROT_TIME/ 20 / (SPEED_TURN_REACHING_MODE - PMW_LOW_SPEED) * ROT_MODE_ROT_TIME);
    stop();
  }
  else if (sensor_number == 1) {
    // turn 30 degrees
    moveRight(SPEED_TURN_REACHING_MODE);
    delay(ROT_MODE_ROT_TIME/ 20 / (SPEED_TURN_REACHING_MODE - PMW_LOW_SPEED) * ROT_MODE_ROT_TIME);
    stop();
  }
}
