#ifndef NATESDISPLAYH
#define NATESDISPLAYH

#include "Arduino.h"

void clearSynchronous();
void clearFifth();
void sendCharToDigit(int _char, byte digitValue);
void sendCharToFifthDigit(int _char);
void natesShiftOut(byte val);
byte getCharAsByte(char c);
void writeCreeper(char * creepMe, byte loops = 1);
void write5(char * fiveChars);
void setupDisplay();

#endif NATESDISPLAYH
