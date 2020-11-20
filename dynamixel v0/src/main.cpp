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

////////////////////////////////////////////TO DO //////////////////////////////////
// try creating a new project -> problem: cannot find include file

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

// int temp;


/*
// LED blinking
void setup()
{
	Serial.begin(9600);
  	Serial.println("Robottle ready to take off");
  	Serial.println("Waiting for command");
	// ax12a.begin(BaudRate, DirectionPin, &Serial1);
	
	// RESET ALL IDS
	// for (int i = 0; i <= 255; i++) {
	//   ax12a.setID(i, ID);
	//   delay(10);
	// }

	// RESET FOR ALL BAUDRATES AND ALL IDS
	// for (int b=1; b<0xFF; b++){                                  // This "for" loop will take about 20 Sec to compelet and is used to loop though all speeds that Dynamixel can be and send reset instuction 
	// 	long Baudrate_BPS = 0;
	// 	Baudrate_BPS  = 2000000 / (b + 1);                        // Calculate Baudrate as ber "Robotis e-manual"
	// 	ax12a.begin(Baudrate_BPS, DirectionPin, &Serial1);   // Set Ardiuno Serial speed and control pin      
	// 	ax12a.reset(0xFE);                                // Broadcast to all Dynamixel IDs(0xFE is the ID for all Dynamixel to responed) and Reset Dynamixel to factory default
	// 	delay(5);     
 	// }
	
	ax12a.begin(1000000,DirectionPin, &Serial1);                 // Set Ardiuno Serial speed to factory default speed of 57600
	delay(10);
	ax12a.setBD(ID,BaudRate);           // Set Dynamixel to new serial speed 
	delay(30);                                                    // Time needed for Dynamixel to set it's new Baudrate
	Serial.println("finished reset");

	ax12a.begin(BaudRate, DirectionPin, &Serial1);    // We now need to set Ardiuno to the new Baudrate speed 
}

void loop()
{
	// temp = ax12a.readTemperature(ID);
	Serial.println("\ninitial pos:");
  	// Serial.println(temp);
	ax12a.ledStatus(ID, ON);
	delay(5000);
	ax12a.ledStatus(ID, OFF);
	delay(1000);
}
/**/



/*
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
*/




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
	delay(1000);
}
/**/




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
