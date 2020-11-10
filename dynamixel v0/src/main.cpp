/*
 * Example of a simple Blink using the AX-12A built-in LED
 */

#include "Arduino.h"
#include "AX12A.h"

//#include <iostream>
// using namespace std;

#define DirectionPin 	(1u)
#define BaudRate  		(1000000ul)
#define ID 						(2u)


/*
void setup()
{
  pinMode(0, INPUT) ;
  pinMode(1, OUTPUT) ;
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
  // reset ID of the thing for literaly every possible adress
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
*/



/*
// LED blinking
void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
}

void loop()
{
	ax12a.ledStatus(ID, ON);
	delay(1000);
	ax12a.ledStatus(ID, OFF);
	delay(1000);
}
*/




// ENDLESS turn
void setup()
{
	// reset ID of the thing for literaly every possible adress
	// for (int i = 0; i <= 255; i++) {
	//   ax12a.setID(i, ID);
	//   delay(10);
	// }
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	ax12a.setEndless(ID, ON);
	ax12a.turn(ID, RIGHT, 500);
}

void loop()
{

}




/*
// Read register
int reg = 0;
// pos from 0 to 0x3ff(300°)
int pos = 200;

void setup()
{
  Serial.begin(9600);
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	ax12a.writeRegister(ID, AX_CCW_ANGLE_LIMIT_L, 0xff);
}

void loop()
{
	reg = ax12a.readRegister(ID, AX_CCW_ANGLE_LIMIT_L, 1);
	Serial.println(reg);
	delay(1000);
}
*/




/*
void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
  delay(1000) ;
  ax12a.move(ID, initial_pos) ;
*/



/*
// move position
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
	//cout << "%d",pos << '\n';
	Serial.print("hid");
	delay(20);
}
*/
