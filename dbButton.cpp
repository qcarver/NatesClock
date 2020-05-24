#include "dbButton.h"
#include "Arduino.h"
#include <avr/interrupt.h>

//Answers Question: is the button waiting to see if it's really been pressed?
//bool isDebouncing = false;
//Answers Question: how many clock interrupts since we started debouncing?
//uint16_t debounceCount = 0;

enum ButtonState{READY, DEBOUNCING, PRESSED, HELD} buttonState;

void pressed(){
  if (buttonState == READY) buttonState == DEBOUNCING;
}

Button::Button(uint8_t _pin) {
  pin = _pin;
  //hopefully it's okay that we have a member fxn as an isr
  attachInterrupt(digitalPinToInterrupt(pin),pressed,HIGH);
}

//debounce is our state pump, it is called by the clock interrupt
void Button::debounce() {
  if (buttonState != READY){
  if (buttonState == DEBOUNCING) {
    debounceCount++;
    if (debounceCount == 2) {
      //debounceCount = 0;
      buttonState = PRESSED;
      Serial.println("Button Pressed");
    }
  }
  if ((buttonState == PRESSED)||(buttonState == HELD))
    if (digitalRead(pin) == HIGH) {
      //Has button remained pressed a little over half a second?
      if (debounceCount >= 7) {
        buttonState = HELD;
      }
    }
    else { //button was pressed, but user let go
      debounceCount = 0;
      buttonState = READY;
    }
  }
  if (debounceCount == 254){
    //abusive user? stuck button, meh..rohkray.
    buttonState = READY;
  }
}

bool Button::isPressed() {
  return buttonState == PRESSED;
}

bool Button::isHeld() {
  return buttonState == HELD;
}
