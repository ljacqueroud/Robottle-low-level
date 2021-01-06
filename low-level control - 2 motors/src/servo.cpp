#include "servo.h"
#include "AX12A.h"
#include "maxon.h"


// position of pickup and release
// setup with measured values. 'setupServo' will update these values
int16_t pos_pickup = 50;
int16_t pos_release = 940;

// positions of closed and open door
int16_t pos_open_door = 800;
int16_t pos_closed_door = 180;


// //////////////////////////// RESET FUNCTIONS //////////////////////////////////////////////////////////

void resetAll(unsigned char ID, long bd, unsigned char DirectionPin, HardwareSerial *srl) {
  /*
  Reset every servo connected, and set baudrate to defined value
  ATTENTION: THIS WILL ALSO RESET THE IDS OF EVERY CONNECTED SERVO
	*/

  for (int b=1; b<0xFF; b++){                       // This "for" loop will take about 20 Sec to compelet and is used to loop though all speeds that Dynamixel can be and send reset instuction 
		long Baudrate_BPS = 0;
		Baudrate_BPS  = 2000000 / (b + 1);              // Calculate Baudrate as ber "Robotis e-manual"
		ax12a.begin(Baudrate_BPS, DirectionPin, srl);   // Begin communication   
		ax12a.reset(0xFE);                                // Broadcast to Dynamixel ID and Reset Dynamixel to factory default
		delay(5);     
 	}

  ax12a.begin(1000000, DirectionPin, srl);
  ax12a.setBD(0xFE, bd);
  ax12a.end();
}

void resetID(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl) {
  /*
  Reset ID of all connected servos to specified ID
  */

	ax12a.begin(BaudRate, DirectionPin, srl);

  for (int i = 0; i <= 255; i++) {
	  ax12a.setID(i, ID);
	  delay(10);
	}

  ax12a.end();
}

void resetBD(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl) {
  for (int b=1; b<0xFF; b++){                                  // This "for" loop will take about 20 Sec to complete and is used to loop though all speeds that Dynamixel can be and send reset instuction 
		long Baudrate_BPS = 0;
		Baudrate_BPS  = 2000000 / (b + 1);                         // Calculate Baudrate as ber "Robotis e-manual"
		ax12a.begin(Baudrate_BPS, DirectionPin, srl);         // Set Ardiuno Serial speed and control pin      
		ax12a.setBD(0xFE,BaudRate);                                // Broadcast to all Dynamixel IDs(0xFE is the ID for all Dynamixel to responed) and Reset Dynamixel to factory default
		delay(5);     
 	}
}

void setupServo(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl, bool showError) {
  /*
  setup communication with servo
  */

  int errorByte;

  //begin com
	ax12a.begin(BaudRate, DirectionPin, srl);
	delay(50);

  // set limit min angle to 0
  errorByte = ax12a.writeRegister2(ID, AX_CW_ANGLE_LIMIT_L, 0x0000);     //set max angle
  if(showError && errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER CW ANGLE LIMIT ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }

  // set limit max angle to 0x03ff
  errorByte = ax12a.writeRegister2(ID, AX_CCW_ANGLE_LIMIT_L, 0x03FF);     //set max angle
  if(showError && errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER CCW ANGLE LIMIT ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }
  
  // set speed of turning
  errorByte = ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, SERVO_MOV_SPEED);     //set speed
  if(showError && errorByte) {
    Serial.println("========== ERROR: CANNOT WRITE REGISTER GOAL SPEED ========== ");
    Serial.print("error byte: ");
    Serial.println(errorByte);
  }
}



// //////////////////////////// MOVE FUNCTION //////////////////////////////////////////////////////////

int moveTo(int ID, int16_t target, uint16_t tor_lim)
{
  /*
  move to given position. speed is set using writeRegister
  */

  int16_t pos;
  int16_t prev_pos;
  uint16_t tor;
  int pos_count = 0;
  int torque_lim_reached = 0;

  ax12a.move(ID, target);
  pos = ax12a.readPosition(ID);
  tor = ax12a.readTorque(ID);
  prev_pos = pos;

  
  // Serial.print(pos);
  // Serial.print(" ");
  // Serial.println(tor);


  while( (pos > (target + POS_EPSILON) || (pos + POS_EPSILON) < target) && pos_count < POS_CHANGE_COUNT && tor < tor_lim ) {

    tor = ax12a.readTorque(ID);
    pos = ax12a.readPosition(ID);

    // counter for checking if arm is stuck
    if(abs(pos - prev_pos) < POS_EPSILON) {
      pos_count ++;
    }
    else {
      pos_count = 0;
      prev_pos = pos;
    }
    

    // check if torque too high
    if((tor > tor_lim)) {
      torque_lim_reached = 1;
      break;
    }

    // Serial.print(pos);
    // Serial.print(" ");
    // Serial.println(tor);

    delay(5);
  }

  return torque_lim_reached;
}


void moveToNoTorque(int ID, int16_t target) {
  /*
  move to given position. speed is set using writeRegister, no torque limit
  */

  int16_t pos;
  int16_t prev_pos;
  int pos_count = 0;

  ax12a.move(ID, target);
  pos = ax12a.readPosition(ID);
  prev_pos = pos;

  
  // Serial.println(pos);


  while( (pos > (target + POS_EPSILON) || (pos + POS_EPSILON) < target) && pos_count < POS_CHANGE_COUNT) {

    pos = ax12a.readPosition(ID);

    // counter for checking if arm is stuck
    if(abs(pos - prev_pos) < POS_EPSILON) {
      pos_count ++;
    }
    else {
      pos_count = 0;
      prev_pos = pos;
    }

    // Serial.println(pos);

    delay(5);
  }

}

// //////////////////////////// ARM SERVO FUNCTIONS //////////////////////////////////////////////////////////

void setupPos(int ID) {
  /*
  move down and up until a torque is felt, keep those positions as pickup and release positions
  */

  // move to middle position
  moveToNoTorque(ID, POS_MID);

  // move to pickup position until torque reaches given limit and save pickup position
  moveTo(ID, POS_PICKUP, TORQUE_LIMIT_SETUP);
  pos_pickup = (ax12a.readPosition(ID) + POS_SETUP_SHIFT);
  moveTo(ID, pos_pickup, TORQUE_LIMIT_HIGH);
  delay(100);

  // move to middle position
  moveToNoTorque(ID, POS_MID);

  // move to release position until torque reaches given limit and save release position
  moveTo(ID, POS_RELEASE, TORQUE_LIMIT_SETUP);
  pos_release = (ax12a.readPosition(ID) - POS_SETUP_SHIFT);
  moveTo(ID, pos_release, TORQUE_LIMIT_HIGH);
}


int movePickup(int ID) {
  /*
  move to pickup position
  returns true if a bottle was detected by the arm
  */
  
  int torque_lim_reached = 0;
  int bottle_pickedup = 0;

  moveToNoTorque(ID, POS_MID);
  torque_lim_reached = moveTo(ID, POS_STANDING_BOTTLE, TORQUE_LIMIT_STANDING_PIKUP);

  if (!torque_lim_reached) {
    torque_lim_reached = moveTo(ID, pos_pickup, TORQUE_LIMIT_PICKUP);
  }

  // if torque limit reached: stop (move to current position)
  if(torque_lim_reached) {
    moveTo(ID, ax12a.readPosition(ID), TORQUE_LIMIT_PICKUP);
  }

  if(ax12a.readPosition(ID) > (pos_pickup + POS_PICKUP_TOL)) {
    bottle_pickedup = 1;
  }

  return bottle_pickedup;
}


int moveRelease(int ID) {
  /*
  move to release position
  */

  int torque_lim_reached;
  
  moveToNoTorque(ID, POS_MID);
  ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, SERVO_MOV_HIGH_SPEED); 
  torque_lim_reached = moveTo(ID, pos_release, TORQUE_LIMIT_RELEASE);
  ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, SERVO_MOV_SPEED); 

  // if torque limit reached: stop (move to current position)
  if(torque_lim_reached) {
    moveTo(ID, ax12a.readPosition(ID), TORQUE_LIMIT_RELEASE);
  }

  return torque_lim_reached;
}


// //////////////////////////// DOOR SERVO FUNCTIONS //////////////////////////////////////////////////////////

void openDoor(int ID) {
  /*
  opens the back door
  */

  moveTo(ID, pos_open_door, TORQUE_LIMIT_DOOR);
}

void closeDoor (int ID) {
  /*
  closes the back door
  */

  moveTo(ID, pos_closed_door, TORQUE_LIMIT_DOOR);
}
