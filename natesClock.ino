nclude "TimerOne.h"
#include "pitches.h"

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

int fifthDigitAnodes[7] = {anodeA_5th, anodeB_5th, anodeC_5th = 12, anodeD_5th,anodeE_5th = 10,anodeF_5th = 2,anodeG_5th = 9};

const byte ZERO  =127-32;
const byte ONE = 2+4;
const byte TWO = 1+2+32+16+8;
const byte THREE = 32+8+4+2+1;
const byte FOUR  =2+4+32+64;
const byte FIVE  =127-2-16;
const byte SIX   =127-2;
const byte SEVEN =1+2+4;
const byte EIGHT =127;
const byte NINE  =127-16;
const byte A = 1+ 2 + 4 + 16 + 32 + 64;
const byte B = 127 -1-2; //little b
const byte C = 1+8+16+64;
const byte D = 2+4+8+16+32; //litle d
const byte E = 1+8+16+32+64;
const byte F = 1+16+32+64;
const byte G = 1+2+4+8+32+64; //little g
const byte H = 127-1-2-8; //little h
const byte I = 2 + 4;
const byte J = 2+4+8;
const byte K = 127-2-8; //kind of a hacked up little k ('lil h bar)
const byte L = 64+16+8;
const byte M = 16+32+4 + 1; //like little n w/ a line over it
const byte N = 127-32-8;
const byte O = 32+16+4+8; //little o
const byte P = 127-4-8;
const byte Q = 127-16-8; //little q
const byte R = 16+32;  //little r
const byte S = 1+64+32+4+8;
const byte T = 64+32+16+8; //little t
const byte U = 127-1-32;
const byte V = 16+8+4; //like little u
const byte W = 1 + 16 + 8 + 4; //hack v with a line over it
const byte X = 127 - 1 - 8;
const byte Y = G - 1; //little y
const byte Z = 1 + 2 + 32 + 16 + 8;
const byte DASH = 32;
const byte VBARS = 1 + 32 + 8;
const byte HBARS = 64+16+2+4;
const byte UNDERSCORE = 8;
const byte COLON = 9;
const byte R1 = 1;
const byte R2 = 2;
const byte R3 = 4;
const byte R4 = 8;
const byte R5 = 16;
const byte R6 = 64;
const byte CR1 = 1+8;
const byte CR2 = 2+16;
const byte CR3 = 4+64;
const byte SPACE = 128;

const byte alphabet[] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
DASH,VBARS,HBARS,UNDERSCORE,R1,R2,R3,R4,R5,R6,CR1,CR2,CR3,
SPACE};
const byte alphabetLen = sizeof(alphabet);

boolean tick = true;
const char sz_tick[] = "tick ";
const char sz_tock[] = "tock ";
int deciseconds = 0;
int seconds = 20;
int minutes = 42;
int hour = 8;
int alarmMinutes = 30;
int alarmHours = 6;

boolean rbDebouncing = false;
boolean lbDebouncing = false;
boolean alarmState = false;

enum Song{BLUE_BIRD, GOOD_MORNING_TO_YOU};


Song currSong = BLUE_BIRD;

enum State{
  TIME,
  PULSE_PLAY_SONG,            PULSE_SCROLL_MSG,
  HOLD_SET_ALARM,             HOLD_SET_TIME,
  SCROLL_SONG_NAME,           SCROLL_A_RANDOM_MESSAGE,
  PLAY_SONG,
  SHOW_ALARM_STATE,
  BLINK_ALARM_TIME,           BLINK_TIME,
  CHANGE_ALARM_TIME};
  
State state = TIME;
  
  

// {NOTE durations: 4 = quarter {NOTE, 8 = eighth {NOTE, etc.:
int NOTEDurations[] ={4, 8, 8, 4,4,4,4,4 };
  
  // {NOTEs in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
  
int  blueBirdSong [][2]= 
  {  {NOTE_E5,8},  {NOTE_GS5,8},  {NOTE_GS5,8},  {NOTE_B5,8},  {NOTE_CS6,8},   {NOTE_B5,8},  {NOTE_B5,4},  {NOTE_E6,8},  {NOTE_B5,8},  {NOTE_B5,8},  {NOTE_GS5,8},  {NOTE_GS5,8},  {NOTE_FS5,16},  {NOTE_E5,16},  {NOTE_FS5,8}, {NOTE_B4,8},
     {NOTE_A5,8},  {NOTE_A5,8},   {NOTE_GS5,8},  {NOTE_GS5,8}, {NOTE_FS5,4},  {NOTE_E5,8},  {NOTE_GS5,16}, {NOTE_A5,16},  {NOTE_B5,8},  {NOTE_B5,8},  {NOTE_B5,8},   {NOTE_A5,8},   {NOTE_GS5,8},  {NOTE_FS5,8},  {NOTE_E5,4}};
    
int blueBirdSongLen = 31;

int goodMorningToYou [][2]=
  {  {NOTE_F4,16},  {NOTE_F4,8}, {NOTE_E5,8},  {NOTE_G5,8}, {NOTE_C6,4}, {NOTE_F4,16},  {NOTE_F4,8}, {NOTE_E5,8}, {NOTE_G5,8},  {NOTE_C6,4}, {NOTE_C6,8},  {NOTE_C6,8}, {NOTE_G5,8},  {NOTE_G5,8},
     {NOTE_G5,8},  {NOTE_E5,8}, {NOTE_C6,8},  {NOTE_C6,8},  {NOTE_G5,8},  {NOTE_G5,8},  {NOTE_G5,8}, {NOTE_E5,8}, {NOTE_E5,8},  {NOTE_F4,8},  {NOTE_E5,8},  {NOTE_G5,8}, {NOTE_C6,4}, {NOTE_C6,8}, 
     {NOTE_C6,8},  {NOTE_G5,8},  {NOTE_E5,8}, {NOTE_F4,4}};
     
char messages[3][52]=
  {"Nate the Great -Dad", "You are the best Nate ever -Mom", "There are no monsters under your bed, just me -Ben"};
     
int goodMorningToYouLen = 32;

void setup() {
  //Timer stuff
  Timer1.initialize(100000); //initialize timer1, and set period to 1/10th of a second
  Timer1.attachInterrupt(incrementTime);
  
  
  //set pins to output so you can control the shift register
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(counterResetPin, OUTPUT);
  pinMode(fifthDigitAnodes[0],OUTPUT);
  pinMode(fifthDigitAnodes[1],OUTPUT);
  pinMode(fifthDigitAnodes[2],OUTPUT);
  pinMode(fifthDigitAnodes[3],OUTPUT);
  pinMode(fifthDigitAnodes[4],OUTPUT);
  pinMode(fifthDigitAnodes[5],OUTPUT);
  pinMode(fifthDigitAnodes[6],OUTPUT);
  pinMode(fifthDigitAnodes[7],OUTPUT);
  //portC   Yes, you can use the analog pins as digital pins, by numbering them 14 - 19.
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(15, OUTPUT);
  clearFifth();
  clearSynchronous();
  
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);

  digitalWrite(15,HIGH);
  delay(500);
  digitalWrite(16,HIGH);
  delay(500);
  digitalWrite(17,HIGH);
  delay(500);
  
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);
  
  delay(500);
  
  //playAlarm();
  //playSong(blueBirdSong, blueBirdSongLen);
  //delay(250);
  //playSong(goodMorningToYou, goodMorningToYouLen);
}

void loop() {
  static int leftButtonPulseCycles = 0;
  static int rightButtonPulseCycles = 0;

  if (state == TIME){
    writeTime();
    if (isLeftButtonPressed) state=PULSE_PLAY_SONG;
  }
  
  else if (state == PULSE_PLAY_SONG){
     if (isLeftButtonPressed()){  
       //button has been held down for a while, promote to hold press
       if (leftButtonPulseCycles++ == 10){
          state=HOLD_SET_ALARM;
          ButtonPulseCycles = 0;
       }
     }
     //button was pulse pressed (in TIME state), and is now released
     else state=SCROLL_SONG_NAME;
  }
    //button was promoted to hold pressed (from PULSE_PLAY_SONG state), and is now release
  else if (state==HOLD_SET_ALARM){
     (!isLeftButtonPressed()){
       //Move to state which shows and querys for alarm ON or OFF
       state=SHOW_ALARM_STATE;
       //If there is no entry after 4 seconds in this state, return to back to TIME state
       inFourSecondsChangeStateTo(TIME);
   }
  }
  
  else if (state==SHOW_ALARM_STATE){
     writeBlink((alarmState)?"AL_ON":"ALOFF");
     
     if(ButtonPressed()){
       alarmState=!alarmState;
       //move on if we don't get user input after while
       if (alarmState=false) InFourSecondsChangeStateTo(TIME);
       else InFourSecondsChangeStateTo(BLINK_ALARM_TIME);
     }
   }
   
   //blink alarm time waits for button presses to increment or decrement and gives up after 4 seconds .. then returns to time
   else if (state==BLINK_ALARM_TIME){
     InFourSecondsChangeStateTo(TIME);
     state=CHANGE_ALARM_TIME;
   }
   
   else if (state==CHANGE_ALARM_TIME){
     if(isLeftButtonPressed()){
       alarmMinutes = ((++alarmMinutes)%60);
       writeAlarmTime();
       InFourSecondsChangeStateTo(TIME);
     } else if (isRightButtonPressed()){
       alarmHours = ((++alarmHours)%13);
       if (alarmHours == 0) alarmHours = 1;
       writeAlarmTime();
       InFourSecondsChangeStateTo(TIME);
     } else blinkAlarmTime();
   }
}

boolean isRightButtonPressed(){
  if ((digitalRead(18) == HIGH) && (!rbDebouncing)){ 
    rbDebouncing = true;
    return true;
  }
  else return false;
}

boolean isLeftButtonPressed(){
  if ((digitalRead(19) == HIGH) && (!rbDebouncing)){ 
    rbDebouncing = true;
    return true;
  }
  else return false;
}

void incrementTime(){
  static int rbDebounceTime = 0;
  static int lbDebounceTime = 0;
  //minimal time for us is a tenth of a second, for button debouncing
  deciseconds++;
  
  if (rbDebouncing){
    rbDebounceTime++;
    if (rbDebounceTime == 2){
      rbDebounceTime = 0;
      rbDebouncing = false;
    }
  }
  
  if (lbDebouncing){
    lbDebounceTime++;
    if (lbDebounceTime == 2){
      lbDebounceTime = 0;
      lbDebouncing = false;
    }
  }
  
  if (deciseconds==10){
    deciseconds = 0;
    seconds ++;
    if (seconds == 60){
      minutes = minutes ++;
      seconds = 0;
      if (minutes == 60){
        hour += 1;
        minutes = 0;
        if (hour == 13){
          hour = 1;
        }        
      }    
    }  
  }
}

void playAlarm(){
    //for tone lib example
    // iterate over the {NOTEs of the melody:
  for (int thisNOTE = 0; thisNOTE < 8; thisNOTE++) {

    // to calculate the {NOTE duration, take one second 
    // divided by the {NOTE type.
    //e.g. quarter {NOTE = 1000 / 4, eighth {NOTE = 1000/8, etc.
    int NOTEDuration = 1000/NOTEDurations[thisNOTE];
    tone(3, melody[thisNOTE],NOTEDuration);

    // to distinguish the {NOTEs, set a minimum time between them.
    // the {NOTE's duration + 30% seems to work well:
    int pauseBetweenNOTEs = NOTEDuration * 1.30;
    delay(pauseBetweenNOTEs);
    // stop the tone playing:
    noTone(3);
  }
}

void playSong(int song[][2], int songLen){
  for (int thisNote = 0; thisNote < songLen; thisNote++){
     // to calculate the {NOTE duration, take one second 
    // divided by the {NOTE type.
    //e.g. quarter {NOTE = 1000 / 4, eighth {NOTE = 1000/8, etc.
    int noteDuration = 1000/song[thisNote][1]*2;
    tone(3, song[thisNote][0], noteDuration);
    // to distinguish the {NOTEs, set a minimum time between them.
    // the {NOTE's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    //stop the tone playing;
    noTone(3);
  }
}

void writeTime(int seconds, int minutes, int hours){
    char sz_time[6] = "     ";
    if (hour > 9){
      sz_time[0]='1';
    }
    else{
      sz_time[0]=' ';
    }
    //fourthDigit
    sz_time[1]=getDigitAsChar(hour%10);
    //thirdDigit ..blinking colon
    sz_time[2]= (seconds%2 == 0)?':':'-';
    //secondDigit
    sz_time[3]=getDigitAsChar((minutes - minutes%10)/10);
    //firstDigit
    sz_time[4]=getDigitAsChar(minutes%10);
    write5(sz_time);
  }
  
  void writeTime(){
    writeTime(seconds, minutes, hour);
  }
  
  void writeAlarmTime(){
    writeTime(0,alarmMinutes,alarmHours);
  }
  
  void blinkAlarmTime(){
    if (deciseconds > 4) writeAlarmTime(); else write5("     ");
  }
  
  void blinkTime(){
     if (deciseconds > 4) writeTime(); else write5("     ");
  }   
    
  
  //call this method on the Hertz change if you want to alternate tick tock
  void writeTickTock(){
    if (tick == true){
        write5("tick");
      tick = false;
    }
    else /*(tick = false)*/{
      write5("tock");
      tick = true;
    }
  }
   

//in Nates counting box we have related 4bit coutner ICs but the 163A's used need
//a special sequence to clear.. see MC74HC163A data sheet figure 12 timing diagram
void clearSynchronous(){
  //load (or PE) wired Hi
  digitalWrite(counterResetPin,1);

  digitalWrite(enablePin,0);
  digitalWrite(counterResetPin,0);
  //delay(500);
  //set every counters clock pin to 1
  shiftOut(dataPin, clockPin, MSBFIRST, 255);

  //set every counters clock pin to 0
  shiftOut(dataPin, clockPin, MSBFIRST, 0);

  digitalWrite(counterResetPin,1);
  //set every counters clock pin to 1
  shiftOut(dataPin, clockPin, MSBFIRST, 255);
  digitalWrite(enablePin,0);
}

//digitValue should be 1 (first --rightmost), 2(second), 4(third), 8(fourth), or 16(fifth -- leftmost)
void sendCharToDigit(int _char, byte digitValue){
 for (int i = 0; i < _char ; i++){
   natesShiftOut(digitValue);
 } 
 if (digitValue & 16){
   sendCharToFifthDigit(_char);
 }
}

//fifth digit is wired right to Arduino port pins
//anode order on spec sheet is slightly different than how we made our alphabet above
void sendCharToFifthDigit(int _char)
{
  if (_char & 1) digitalWrite(anodeA_5th,1); else digitalWrite(anodeA_5th,0);
  if (_char & 2) digitalWrite(anodeB_5th,1); else digitalWrite(anodeB_5th,0);
  if (_char & 4) digitalWrite(anodeC_5th,1); else digitalWrite(anodeC_5th,0);
  if (_char & 8) digitalWrite(anodeD_5th,1); else digitalWrite(anodeD_5th,0);
  if (_char & 16) digitalWrite(anodeE_5th,1); else digitalWrite(anodeE_5th,0);
  if (_char & 32) digitalWrite(anodeG_5th,1); else digitalWrite(anodeG_5th,0);
  if (_char & 64) digitalWrite(anodeF_5th,1); else digitalWrite(anodeF_5th,0);
  return;
}

void clearFifth()
{
  for (int i = 0; i < 7; i ++){
    digitalWrite(fifthDigitAnodes[i], 0);
  }
}

//this is a custom shift out for Nates counting box, 
//(1) we always use the same clock pin, (2) we are always MSB
void natesShiftOut(byte val)
{  
      for (int i = 0; i < 8; i++)  {
            //reset the enable either for the next iteration, or next function call   
            digitalWrite(clockPin, LOW);                  
            digitalWrite(dataPin, (val & (1 << (7 - i))));
            
            //8th bit has shifted out  .. enable the 74C08 AND gate
            if (i == 7){
              digitalWrite(enablePin, HIGH);
           }

             //clockPin only affects the 4-bit cascaded 74LS16*
            digitalWrite(clockPin, HIGH);
            digitalWrite(enablePin, LOW);
      }
}

void writeCreeper(char * creepMe, int len, int loops){
  for (int j = 0; j < loops; j++){
  for (int i = 0; i < len ; i ++){
   sendCharToDigit(getCharAsByte(creepMe[i%len]),16);
   sendCharToDigit(getCharAsByte(creepMe[(i+1)%len]),8);
   sendCharToDigit(getCharAsByte(creepMe[(i+2)%len]),4);
   sendCharToDigit(getCharAsByte(creepMe[(i+3)%len]),2);
   sendCharToDigit(getCharAsByte(creepMe[(i+4)%len]),1);
   delay(200);
   clearSynchronous();
  }
  }
}

void write5(char * fiveChars){
  int len = 5;
  
   sendCharToDigit(getCharAsByte(fiveChars[0]),16);
   sendCharToDigit(getCharAsByte(fiveChars[1]),8);
   sendCharToDigit(getCharAsByte(fiveChars[2]),4);
   sendCharToDigit(getCharAsByte(fiveChars[3]),2);
   sendCharToDigit(getCharAsByte(fiveChars[4]),1);
}

//convert a single digit to it's character value (eg 0 becomes '0')
char getDigitAsChar(int i){
  //if its not a single digit.. just force it into one
  i = i%10;
  
  //ASCII table starts 0-9 at 0x30
  char c = (char)i + 0x30;
  
  return c;
}

byte getCharAsByte(char c){
  byte b = HBARS;
 switch(c) {
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
  

void writeCreeper(byte* creepMe, int len, int loops){
  for (int j = 0; j < loops; j++){
  for (int i = 0; i < len ; i ++){
   sendCharToDigit(creepMe[i%len],16);
   sendCharToDigit(creepMe[(i+1)%len],8);
   sendCharToDigit(creepMe[(i+2)%len],4);
   sendCharToDigit(creepMe[(i+3)%len],2);
   sendCharToDigit(creepMe[(i+4)%len],1);
   delay(200);
   clearSynchronous();
  }
  }

}





