#ifndef DBBUTTONH
#define DBBUTTONH
//for uint8_t typedef
#include "Arduino.h"

class Button{
  public:
  Button(uint8_t pin);
  bool isPressed();
  bool isHeld();
  void debounce();

  //shhh!
  private:
  //The pin is this button wired to, # clock interrupts since start of debounce
  uint8_t pin, debounceCount = 0;
  //Answers Question: is the button waiting to see if it's really been pressed?
  bool isDebouncing = false;
  //disable default contructor b/c can't really construct w/o a pin
  Button();
};

#endif

