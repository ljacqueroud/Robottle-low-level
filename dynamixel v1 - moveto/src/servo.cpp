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
