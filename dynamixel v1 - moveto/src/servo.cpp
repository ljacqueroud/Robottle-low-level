#include "servo.h"
#include "AX12A.h"


uint16_t pos_pickup;
uint16_t pos_release;


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

void reserBD(unsigned char ID, long BaudRate, unsigned char DirectionPin, HardwareSerial *srl) {
  for (int b=1; b<0xFF; b++){                                  // This "for" loop will take about 20 Sec to complete and is used to loop though all speeds that Dynamixel can be and send reset instuction 
		long Baudrate_BPS = 0;
		Baudrate_BPS  = 2000000 / (b + 1);                         // Calculate Baudrate as ber "Robotis e-manual"
		ax12a.begin(Baudrate_BPS, DirectionPin, srl);         // Set Ardiuno Serial speed and control pin      
		ax12a.setBD(0xFE,BaudRate);                                // Broadcast to all Dynamixel IDs(0xFE is the ID for all Dynamixel to responed) and Reset Dynamixel to factory default
		delay(5);     
 	}
}



// //////////////////////////// MOVE FUNCTIONS //////////////////////////////////////////////////////////

int moveTo(int ID, uint16_t target, uint16_t tor_lim)
{
  /*
  move to given position. speed is set using writeRegister
  */

  uint16_t pos;
  uint16_t tor;
  int torque_lim_reached = 0;
  int torque_lim_count = 0;

  ax12a.move(ID, target);
  pos = ax12a.readPosition(ID);

  while((pos > (target + POS_EPSILON) || (pos + POS_EPSILON) < target)) {

    tor = ax12a.readTorque(ID);

    // counter for checking if arm is stuck
    if(tor > TORQUE_LIMIT_LOW) {
      torque_lim_count ++;
    }
    else {
      torque_lim_count = 0;
    }

    // check if torque too high
    if((ax12a.readTorque(ID) > tor_lim) || torque_lim_count > TORQUE_LIMIT_COUNT) {
      torque_lim_reached = 1;
      break;
    }

    pos = ax12a.readPosition(ID);    

    // Serial.print("load: ");
    // Serial.println(ax12a.readTorque(ID));

    delay(5);
  }

  return torque_lim_reached;
}

void setupPos(int ID) {
  // move to middle position
  moveTo(ID, POS_MID, TORQUE_LIMIT_HIGH);

  // move to pickup position until torque reaches given limit and save pickup position
  moveTo(ID, POS_PICKUP, TORQUE_LIMIT_SETUP);
  Serial.println(pos_pickup = (ax12a.readPosition(ID) + POS_SETUP_SHIFT));
  moveTo(ID, pos_pickup, TORQUE_LIMIT_HIGH);
  delay(100);

  // move to middle position
  moveTo(ID, POS_MID, TORQUE_LIMIT_HIGH);

  // move to release position until torque reaches given limit and save release position
  moveTo(ID, POS_RELEASE, TORQUE_LIMIT_SETUP);
  Serial.println(pos_release = (ax12a.readPosition(ID) - POS_SETUP_SHIFT));
  moveTo(ID, pos_release, TORQUE_LIMIT_HIGH);
}


int movePickup(int ID) {

  int torque_lim_reached;
  int bottle_pickedup = 0;

  moveTo(ID, POS_MID, TORQUE_LIMIT_HIGH);
  torque_lim_reached = moveTo(ID, pos_pickup, TORQUE_LIMIT_PICKUP);

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

  int torque_lim_reached;
  
  moveTo(ID, POS_MID, TORQUE_LIMIT_HIGH);
  torque_lim_reached = moveTo(ID, pos_release, TORQUE_LIMIT_RELEASE);

  // if torque limit reached: stop (move to current position)
  if(torque_lim_reached) {
    moveTo(ID, ax12a.readPosition(ID), TORQUE_LIMIT_RELEASE);
  }

  return torque_lim_reached;
}