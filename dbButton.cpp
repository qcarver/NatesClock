#include "Arduino.h"
#include <avr/interrupt.h>
#include "TimerOne.h"
#include "dbButton.h"

/*Written for my Arduino Uno which only can assign interrupts to 2 or 3
 * Circuit:
 * 5V to 
 *  4.7u INTERRUPT_CAP anode(+)
 *  and LEFT_BUTTON_IN
 *  and RIGHT_BUTTON_IN
 * LEFT_BUTTON_OUT to
 *  4.7u LEFT_CAP anode(+)
 *  black end of LEFT_DIODE
 *  left digital pin
 * RIGHT_BUTTON_OUT to
 *  4.7u RIGHT_CAP anode(+)
 *  black end of RIGHT_DIODE
 *  right digital pin 
 * white ends of both RIGHT_DIODE & LEFT_DIODE
 *  (each other)
 *  white end of RIGHT_DIODE
 *  10k Resistor to ground
 *  4.7u INTERRUPT_CAP cathode(-)
 *  the interrupt pin
 
 *  
 *  Use case: a circuit that has two buttons each on there own digital pin
 *  but they also share an interrupt pin. (3 pins used total)
 */
//#include "pins_arduino.h"
volatile unsigned long highTime, lowTime; //uSecs
volatile bool rightButtonPinHigh = false, leftButtonPinHigh = false;
const int interruptPin = 3;
const int leftButtonPin = 12;
const int rightButtonPin = 13;
const int leftLed = 11;
const int centerLed = 10;
const int rightLed = 9;

volatile enum ButtonState {READY, PRESSING, PRESSED, HELD} buttonState = READY;

//if you hold for this long, then a button press becomes a button hold
const int holdTime = 400; //uSecs


/*Interrupt Service Routine...keep it simple*/
void button() {

    rightButtonPinHigh = digitalRead(rightButtonPin);
    leftButtonPinHigh = digitalRead(leftButtonPin);
  
  //reset the count..to be in this state we are low going high
  if(buttonState == READY){
    lowTime=millis()-1;
  }

  (digitalRead(interruptPin)==HIGH)? highTime=millis(): lowTime=millis();
  
  //remember interrupt only sees change, not steady state, this limits our cases
  if (lowTime < highTime){
    buttonState = PRESSING;
  }
  else{ //(lowTime > highTime)

    if (lowTime - highTime > holdTime){
      //If API hasn't already noticed make it official, otw reset.
      buttonState = (buttonState == READY)?READY:HELD;
    }
    else buttonState = PRESSED;
  }
}

void setupButtons(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("*** ");Serial.print(__FILE__);Serial.print(" Build: ");
  Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__);
  Serial.println(" ***");
  pinMode(interruptPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  /*pinMode(leftLed,INPUT);
  digitalWrite(leftLed, LOW);
  pinMode(centerLed,INPUT);
  digitalWrite(centerLed, LOW);
  pinMode(rightLed, INPUT);
  digitalWrite(rightLed, LOW);*/
  attachInterrupt(digitalPinToInterrupt(interruptPin), button, CHANGE);
}

bool isPressed(){
  bool rv = (buttonState == PRESSED);
  //once we have reported the button state reset it ...//also check for false alarm (bad hw)
  if (rv == true) buttonState = READY;
  return rv;
}

bool Button::isRPressed(){
  return (rightButtonPinHigh&&isPressed());
}

bool Button::isLPressed(){
  return (leftButtonPinHigh&&isPressed());
}

bool isHeld(){
  //consider the button held if it has been held down too long
  if (buttonState == PRESSING){
    if ((millis() - highTime >= holdTime)&&(highTime > lowTime)){
      buttonState = HELD;
    }
  }
  //also consider if the ISR marked it as such
  bool rv = (buttonState == HELD);
  //once we have reported the button state reset it
  if (rv == true) buttonState = READY;
  return rv;
}

bool Button::isRHeld(){
  return (rightButtonPinHigh&&isHeld());
}

bool Button::isLHeld(){
  return (leftButtonPinHigh&&isHeld());
}

bool isPressing(){
  //report, but stay in this state
  return (buttonState == PRESSING);
}

//#define NATESCLOCK_VERBOSE
void Button::print(){
  bool action = false;
  if (Button::isLHeld()){
    Serial.println("left button pressed and held");
    action = true;
  }

  else if (Button::isRHeld()){
    Serial.println("right button pressed and held");
    action = true;
  }
  
  else if (Button::isLPressed()){
    Serial.println("left button pressed");
    action = true;
  }

  else if (Button::isRPressed()){
    Serial.println("right button pressed");
    action = true;
  }
#ifdef NATESCLOCK_VERBOSE 
//having trouble w/ interrupt firing and nothing else hi, bad hw 
 if (rightButtonPinHigh || rightButtonPinHigh){
/**/  Serial.print("RightButtonPinHi: "); Serial.print(rightButtonPinHigh);
/**/  Serial.print("LeftButtonPinHi: "); Serial.print(leftButtonPinHigh);
/**/  Serial.print(" TimeHi: "); Serial.print(highTime);
/**/  Serial.print(" TimeLo: "); Serial.print(lowTime);
/**/  Serial.print(" Delta: ");
/**/  if (highTime > lowTime){
/**/    Serial.print(highTime - lowTime);
/**/  } else {
/**/    Serial.print("-");
/**/    Serial.print(lowTime - highTime);
/**/  }
/**/  Serial.print(" millis: "); Serial.print(millis());
/**/  Serial.print(" state: ");Serial.println(buttonState);
 }
#endif
}
