/*
 * Example of a simple Blink using the AX-12A built-in LED
 */

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AX12A.h"
#include "servo.h"

//#include <iostream>
// using namespace std;

#define DirectionPin 	(1u)
#define BaudRate  		(200000ul)
#define ID 						(2u)



// /*
//pos: 0=min, 1023=max for range of 300° (10°=34.1)
int posRest = 580;        //-20°
int posGrab = 205;         //90°

//speed: 1=min, 1023=max
int movSpeed = 150;

int currentPos = 512;

int reg;


void setup()
{
  Serial.begin(9600);
	ax12a.begin(BaudRate, DirectionPin, &Serial1);      //begin com, ATTENTION: make sure servo has correct ID and baudrate
  ax12a.writeRegister(ID, AX_CCW_ANGLE_LIMIT_L, 0xff);


  // ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, movSpeed);    //set speed

  // currentPos = ax12a.readRegister(ID, AX_PRESENT_POSITION_L, 2);  //read initial pos
  // Serial.print("\ninitial pos:");
  // Serial.println(currentPos);

  // moveto(ID, posRest);        //move to rest position
  // delay(500);
}

void loop()
{
//   moveto(ID, posGrab);
//   currentPos = ax12a.readRegister(ID, AX_PRESENT_VOLTAGE, 1);
//   Serial.print("servo arrived at pos grab:");
//   Serial.println(currentPos);
//   delay(500);

//   moveto(ID, posRest);
//   currentPos = ax12a.readRegister(ID, AX_PRESENT_VOLTAGE, 1);
//   Serial.print("servo arrived at pos rest:");
//   Serial.println(currentPos);
//   delay(500);

  // currentPos = ax12a.readTemperature(ID);
  // currentPos = ax12a.readRegister(ID, AX_PRESENT_POSITION_L, 2);  //read initial pos
  // Serial.print("current pos:");
  // Serial.println(currentPos);

  // ax12a.ledStatus(ID, ON);
  // delay(500);
  // ax12a.ledStatus(ID,OFF);
  // delay(500);


  reg = ax12a.readRegister(ID, AX_CCW_ANGLE_LIMIT_L, 1);
	Serial.println(reg);
	delay(1000);
}
/**/


/*
void setup()
{
  // reset baudrate of servo ID to defined value
  // resetBD(ID, BaudRate, DirectionPin, &Serial1);

	ax12a.begin(BaudRate, DirectionPin, &Serial1);

}

void loop()
{
	ax12a.ledStatus(ID, ON);
	delay(1000);
	ax12a.ledStatus(ID, OFF);
	delay(1000);
}
/**/



/*
// LED BLINK
void setup()
{
  // TO CHANGE BAUDRATE FROM OLD VALUE (1000000) TO NEW VALUE (BaudRate)
	// ax12a.begin(1000000, DirectionPin, &Serial1);
  // ax12a.setBD(ID, BaudRate);
  // ax12a.end();
  ax12a.begin(BaudRate, DirectionPin, &Serial1);

  // //reset ID of the thing for literaly every possible adress
  // for (int i = 0; i <= 255; i++) {
  //   ax12a.setID(i, ID) ;
  //   delay(10);
  // }
}

void loop()
{
	ax12a.ledStatus(ID, ON);
	delay(1000);
	ax12a.ledStatus(ID, OFF);
	delay(1000);
}
/**/



/*
// ENDLESS TURN
void setup()
{
	// reset ID of the thing for literaly every possible adress
	// for (int i = 0; i <= 255; i++) {
	//   ax12a.setID(i, ID);
	//   delay(10);
	// }
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	ax12a.setEndless(ID, ON);
	//ax12a.turn(ID, RIGHT, 100);
  ax12a.writeRegister2(ID, AX_GOAL_SPEED_L, 100);
}

void loop()
{

}
*/



/*
// READ REGISTER
int reg = 0;
// pos from 0 to 0x3ff(300°)
int pos = 15;

void setup()
{
  Serial.begin(9600);
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	//Serial.println(ax12a.writeRegister(ID, AX_CCW_ANGLE_LIMIT_L, 0xff));
  //ax12a.writeRegister(ID, AX_CCW_ANGLE_LIMIT_L, 200);
  //Serial.println(ax12a.move(ID, pos));

  // ax12a.reset(ID);
  // // reset ID of the thing for literaly every possible adress
  // for (int i = 0; i <= 255; i++) {
  //   ax12a.setID(i, ID) ;
  //   delay(10);
  // }

  //ax12a.writeRegister2(ID, AX_GOAL_POSITION_L, pos);
}

void loop()
{
	// reg = ax12a.readRegister(ID, AX_CCW_ANGLE_LIMIT_L, 1);
  // //Serial.print("Register:");
	// Serial.println(reg);
  //reg = ax12a.readRegister(ID, AX_CW_ANGLE_LIMIT_L, 1);

  ax12a.writeRegister2(ID, AX_GOAL_POSITION_L, pos);
  delay(200);
  reg = ax12a.readRegister(ID, AX_GOAL_POSITION_L, 1);


  Serial.println(reg,HEX);
	delay(1000);
}
*/






/*
// MOVE TO POSITION
int initial_pos = 512;
int max = initial_pos + 100;
int min = initial_pos - 100;

int pos = initial_pos;
int delta = 5;

void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
}

void loop()
{
	pos = pos + delta;

	if (pos > max)
	{
		pos = max;
		delta *= -1;
	}

	if (pos < min)
	{
		pos = min;
		delta *= -1;
	}

	ax12a.move(ID, pos);
	delay(20);
}
*/


/*
// SET NEW BAUD RATE
long newBaud = 1000000;

void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
  ax12a.setBD(ID, newBaud);
}

void loop()
{

}
*/



