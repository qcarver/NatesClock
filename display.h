#ifndef NATESDISPLAYH
#define NATESDISPLAYH

#include "Arduino.h"

enum LED_PATTERN{
  NONE = 0, LEFT = 1, MIDDLE = 2, MID = 2, CENTER = 2, GREEN_ONES = 3, ALARM_OFF = 3, RIGHT = 4, RED = 4, ALARM_ON = 4, ALL = 7 
};

void setPanelLeds(LED_PATTERN ledPattern);
void clearSynchronous();
void clearFifth();
void sendCharToDigit(int _char, byte digitValue);
void sendCharToFifthDigit(int _char);
void natesShiftOut(byte val);
byte getCharAsByte(char c);
bool sideScroll(char * scrollMe);
void write5(char * fiveChars);
void setupDisplay();

#endif NATESDISPLAYH
