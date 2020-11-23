/*
 * Example of a simple Blink using the AX-12A built-in LED
 */

#include "Arduino.h"
#include "AX12A.h"

//#include <iostream>
// using namespace std;

#define DirectionPin 	(1u)
#define BaudRate  		(200000ul)
#define ID 				(2u)



// int temp;


/*
// LED blinking
void setup()
{
	Serial.begin(9600);
  	Serial.println("Robottle ready to take off");
  	Serial.println("Waiting for command");

	ax12a.begin(BaudRate, DirectionPin, &Serial1);    // We now need to set Ardiuno to the new Baudrate speed 
}

void loop()
{
	ax12a.ledStatus(ID, ON);
	delay(5000);
	ax12a.ledStatus(ID, OFF);
	delay(1000);
}
/**/



// /*
// ENDLESS turn
void setup()
{
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	ax12a.setEndless(ID, ON);
	ax12a.turn(ID, LEFT, 200);
}

void loop()
{

}
/**/



/*
// Read register
int reg = 0;
// pos from 0 to 0x3ff(300°)
int pos = 200;

void setup()
{
  	Serial.begin(9600);
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
	// ax12a.writeRegister(ID, AX_CCW_ANGLE_LIMIT_L, 0xff);
}

void loop()
{
	reg = ax12a.readRegister(ID, AX_CCW_ANGLE_LIMIT_L, 1);
	Serial.println(reg);
}
/**/


/*
// Ping
int reg;

void setup()
{
  	Serial.begin(9600);
	ax12a.begin(BaudRate, DirectionPin, &Serial1);
}

void loop()
{
	reg = ax12a.ping(ID);
	Serial.println(reg);
	delay(500);
}
/**/




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
	delay(20);
}
/**/
