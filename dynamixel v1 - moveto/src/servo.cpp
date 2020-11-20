#include "servo.h"
#include "AX12A.h"


void moveto(int id, int target)
{
  /*
  int delta = 1;

  if(target<current){
    delta *= -1;
  }

  while(target != current){
    current += delta;
    ax12a.move(id, current);
  }
  */

  int pos;
  int epsilon = 1;

  ax12a.move(id, target);
  pos = ax12a.readRegister(id, AX_PRESENT_POSITION_L, 2);
  // if(pos<0) {
  //   Serial.print("ERROR: cant read position ");
  //   Serial.println(pos);
  // }
  // else {
  //   Serial.print("current position: ");
  //   Serial.println(pos);
  // }

  while(pos > target+epsilon || pos < target-epsilon) {
    delay(40);
    pos = ax12a.readRegister(id, AX_PRESENT_POSITION_L, 2);
    //Serial.print("load: ");
    //Serial.println(ax12a.readRegister(id, AX_PRESENT_LOAD_L, 2));
    
    // if(pos<0) {
    //   Serial.print("ERROR: cant read position ");
    //   Serial.println(pos);
    // }
    // else {
    //   Serial.print("current position: ");
    //   Serial.println(pos);
    // }
  }
}


void resetBD(unsigned char ID, long bd, unsigned char DirectionPin, HardwareSerial *srl) {
  // ATTENTION: THIS WILL ALSO RESET THE IDS OF EVERY CONNECTED SERVO
	// reset all 
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
