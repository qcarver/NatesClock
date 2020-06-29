#include "display.h"
#include "Arduino.h"
#include "nates_symbols.h"

//
//Pin connected to input B of 74LS164
int dataPin = 7;
//Pin connected to clock pin of 74LS164
int clockPin = 5;
//Pin connected to CET & PE on 74ls161 ...enable before last shift
int enablePin = 6;
//Reset 74ls161 binary counters when low
int counterResetPin = 4;

//fifth digit is different, tied to Arduino port pins
int anodeA_5th = 8;
int anodeB_5th = 13;
int anodeC_5th = 12;
int anodeD_5th = 11;
int anodeE_5th = 10;
int anodeF_5th = 2;
int anodeG_5th = 9;

int fifthDigitAnodes[7] = {anodeA_5th, anodeB_5th, anodeC_5th = 12, anodeD_5th, anodeE_5th = 10, anodeF_5th = 2, anodeG_5th = 9};

byte oldChar[5] = {SPACE, SPACE, SPACE, SPACE, SPACE};

//fwd declare.. object private
void clearSynchronous();
void clearFifth();
void sendCharToDigit(int _char, byte digitValue);
void sendCharToFifthDigit(int _char);
void natesShiftOut(byte val);
byte getCharAsByte(char c);
void writeCreeper(char * creepMe, int len, int loops);

void setupDisplay() {
  //set pins to output so you can control the shift register
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(counterResetPin, OUTPUT);
  pinMode(fifthDigitAnodes[0], OUTPUT);
  pinMode(fifthDigitAnodes[1], OUTPUT);
  pinMode(fifthDigitAnodes[2], OUTPUT);
  pinMode(fifthDigitAnodes[3], OUTPUT);
  pinMode(fifthDigitAnodes[4], OUTPUT);
  pinMode(fifthDigitAnodes[5], OUTPUT);
  pinMode(fifthDigitAnodes[6], OUTPUT);
  pinMode(fifthDigitAnodes[7], OUTPUT);
  //portC   Yes, you can use the analog pins as digital pins, by numbering them 14 - 19.
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(15, OUTPUT);
  clearFifth();
  clearSynchronous();
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  digitalWrite(15, HIGH);
  delay(500);
  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(17, HIGH);
  delay(500);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  delay(500);
}

//in Nates counting box we have related 4bit coutner ICs but the 163A's used need
//a special sequence to clear.. see MC74HC163A data sheet figure 12 timing diagram
void clearSynchronous() {
  //load (or PE) wired Hi
  digitalWrite(counterResetPin, 1);

  digitalWrite(enablePin, 0);
  digitalWrite(counterResetPin, 0);
  //delay(500);
  //set every counters clock pin to 1
  shiftOut(dataPin, clockPin, MSBFIRST, 255);

  //set every counters clock pin to 0
  shiftOut(dataPin, clockPin, MSBFIRST, 0);

  digitalWrite(counterResetPin, 1);
  //set every counters clock pin to 1
  shiftOut(dataPin, clockPin, MSBFIRST, 255);
  digitalWrite(enablePin, 0);
}

//digitValue should be 1 (first --rightmost), 2(second), 4(third), 8(fourth), or 16(fifth -- leftmost)
void sendCharToDigit(int _char, byte digitValue) {
  //safety first
  digitValue &= 0xFF;

  //if it's the fifth digit, it's just a parallel write, no flicker, so who cares
  if (digitValue & 16) {
    sendCharToFifthDigit(_char);
    oldChar[5] = _char;
  }
  //this digit (1 through 4) is made by counting anodes up to the value
  else {
    int whichDigit = 5;

    for (int bits = digitValue; bits > 0; bits >>= 1) {
      whichDigit--;
    }

    //only write the new char if it has changed
    if (oldChar[whichDigit] != _char) {
      byte rollover = 256 - oldChar[whichDigit];

     /*DEBUG
      Serial.print("Setting Digit [");
      Serial.print(whichDigit);
      Serial.print("] from ");
      Serial.print(oldChar[whichDigit]);
      Serial.print(" to "); Serial.println(_char);*/

      for (int i = 0; i < rollover ; i++) {
        //shift in new value starting from zero
        natesShiftOut(digitValue);
      }

      for (int i = 0; i < _char ; i++) {
        //shift in new value starting from zero
        natesShiftOut(digitValue);
      }
      oldChar[whichDigit] = _char;
    }
  }
}

//fifth digit is wired right to Arduino port pins
//anode order on spec sheet is slightly different than how we made our alphabet above
void sendCharToFifthDigit(int _char)
{
  if (_char & 1) digitalWrite(anodeA_5th, 1); else digitalWrite(anodeA_5th, 0);
  if (_char & 2) digitalWrite(anodeB_5th, 1); else digitalWrite(anodeB_5th, 0);
  if (_char & 4) digitalWrite(anodeC_5th, 1); else digitalWrite(anodeC_5th, 0);
  if (_char & 8) digitalWrite(anodeD_5th, 1); else digitalWrite(anodeD_5th, 0);
  if (_char & 16) digitalWrite(anodeE_5th, 1); else digitalWrite(anodeE_5th, 0);
  if (_char & 32) digitalWrite(anodeG_5th, 1); else digitalWrite(anodeG_5th, 0);
  if (_char & 64) digitalWrite(anodeF_5th, 1); else digitalWrite(anodeF_5th, 0);
  return;
}

void clearFifth()
{
  for (int i = 0; i < 7; i ++) {
    digitalWrite(fifthDigitAnodes[i], 0);
  }
}

//this is a custom shift out for Nates counting box,
//(1) we always use the same clock pin, (2) we are always MSB
void natesShiftOut(byte val)
{
  //Serial.print(val);
  if (!val) return;
  for (int i = 0; i < 8; i++)  {
    //reset the enable either for the next iteration, or next function call
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, (val & (1 << (7 - i))));

    //8th bit has shifted out  .. enable the 74C08 AND gate
    if (i == 7) {
      digitalWrite(enablePin, HIGH);
    }

    //clockPin only affects the 4-bit cascaded 74LS16*
    digitalWrite(clockPin, HIGH);
    digitalWrite(enablePin, LOW);
  }
}


void writeCreeper(byte* creepMe, int len, int loops) {
      clearSynchronous();
  for (int j = 0; j < loops; j++) {
    for (int i = 0; i < len ; i ++) {
      sendCharToDigit(creepMe[i % len], 16);
      sendCharToDigit(creepMe[(i + 1) % len], 8);
      sendCharToDigit(creepMe[(i + 2) % len], 4);
      sendCharToDigit(creepMe[(i + 3) % len], 2);
      sendCharToDigit(creepMe[(i + 4) % len], 1);
      delay(200);
      clearSynchronous();
    }
  }
      clearSynchronous();
}

byte getCharAsByte(char c) {
  byte b = HBARS;
  switch (c) {
    case '0': b = ZERO; break;
    case '1': b = ONE; break;
    case '2': b = TWO; break;
    case '3': b = THREE; break;
    case '4': b = FOUR; break;
    case '5': b = FIVE; break;
    case '6': b = SIX; break;
    case '7': b = SEVEN; break;
    case '8': b = EIGHT; break;
    case '9': b = NINE; break;
    case 'a':;
    case 'A': b = A; break;
    case 'b':;
    case 'B': b = B; break;
    case 'c':;
    case 'C': b = C; break;
    case 'd': ;
    case 'D': b = D; break;
    case 'e':;
    case 'E': b = E; break;
    case 'f':;
    case 'F': b = F; break;
    case 'g':;
    case 'G': b = G; break;
    case 'h':;
    case 'H': b = H; break;
    case 'i':;
    case 'I': b = I; break;
    case 'j':;
    case 'J': b = J; break;
    case 'k':;
    case 'K': b = K; break;
    case 'l':;
    case 'L': b = L; break;
    case 'm':;
    case 'M': b = M; break;
    case 'n':;
    case 'N': b = N; break;
    case 'o':;
    case 'O': b = O; break;
    case 'p':;
    case 'P': b = P; break;
    case 'q':;
    case 'Q': b = Q; break;
    case 'r':;
    case 'R': b = R; break;
    case 's':;
    case 'S': b = S; break;
    case 't':;
    case 'T': b = T; break;
    case 'u':;
    case 'U': b = U; break;
    case 'v':;
    case 'V': b = V; break;
    case 'w':;
    case 'W': b = W; break;
    case 'x':;
    case 'X': b = X; break;
    case 'y':;
    case 'Y': b = Y; break;
    case 'z':;
    case 'Z': b = Z; break;
    case ',': b = R4; break;
    case '\'': b = R6; break;
    case ' ': b = SPACE; break;
    case '-': b = DASH; break;
    case '_': b = UNDERSCORE; break;
    case ':': b = COLON; break;
  }
  return b;
}

void writeCreeper(char * creepMe, int len, int loops) {
  //convert msg chars  7-segment byte patterns
  for (int i = 0; i < len; i++) {
    creepMe[i % len] = getCharAsByte(creepMe[i % len]);
    creepMe[(i + 1) % len] = getCharAsByte(creepMe[(i + 1) % len]);
    creepMe[(i + 2) % len] = getCharAsByte(creepMe[(i + 2) % len]);
    creepMe[(i + 3) % len] = getCharAsByte(creepMe[(i + 3) % len]);
    creepMe[(i + 4) % len] = getCharAsByte(creepMe[(i + 4) % len]);
  }
  //delegate to byte * version of function
  writeCreeper((byte *)creepMe, len, loops);
}

void write5(char * fiveChars) {
  int len = 5;
  //clearFifth();
  //clearSynchronous();
  sendCharToDigit(getCharAsByte(fiveChars[0]), 16);
  sendCharToDigit(getCharAsByte(fiveChars[1]), 8);
  sendCharToDigit(getCharAsByte(fiveChars[2]), 4);
  sendCharToDigit(getCharAsByte(fiveChars[3]), 2);
  sendCharToDigit(getCharAsByte(fiveChars[4]), 1);
  Serial.println();
}
