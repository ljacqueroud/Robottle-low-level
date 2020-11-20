
#ifndef SERVO_h
#define SERVO_h

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

void moveto(int id, int target);

void resetBD(unsigned char ID, long bd, unsigned char DirectionPin, HardwareSerial *srl);


#endif
