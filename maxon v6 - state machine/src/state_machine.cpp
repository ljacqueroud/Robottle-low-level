#include "state_machine.h"
#include "maxon.h"




int controlMode (int state) {
  /*
  reads the commands and sends speed to SERIAL 
  */

  int readSpeed;

  // reads commands
  if(SERIAL.available()){
    char val = SERIAL.read();
    
    if(val != -1)
    {
      switch(val)
      {
      case 'w'://Move Forward
        moveForward ();
        break;
      case 's'://Move Backward
        moveBackward ();
        break;
      case 'a'://Turn Left
        moveLeft ();
        break;
      case 'd'://Turn Right
        moveRight ();
        break;
      case 'x':
        stop();
        break;
      case 'r':
        state = ROTATION_MODE;
      }
    }
    else stop();
  }

  // reads speeds
  readSpeed = readSpeedL();
  SERIAL.print("l");
  SERIAL.println(readSpeed);
  
  readSpeed = readSpeedR();
  SERIAL.print("r");
  SERIAL.println(readSpeed);

  return state;
  
}



int rotationMode (int state) {
  /*
  rotation of 360°, then returns to state CONTROL_MODE
  */

  SERIAL.print("s");
  SERIAL.println(TASK_IN_PROGRESS);

  moveRight();
  delay(SPEED_TURN * ROT_CONST);
  stop();

  SERIAL.print("s");
  SERIAL.println(TASK_SUCCEDED);

  state = CONTROL_MODE;
}