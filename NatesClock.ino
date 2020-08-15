//use Sketch/IncludeLibrary/ManageLibraries to install TimerOne
//TimeOne: https://playground.arduino.cc/Code/Timer1/
#include "TimerOne.h"
#include "pitches.h"
#include "nates_symbols.h"
#include "messages.h"
#include "songs.h"
#include "display.h"
#include "dbButton.h"

#define SPKR_PIN A0
bool tick = true;
const char sz_tick[] = "tick ";
const char sz_tock[] = "tock ";
uint16_t deciseconds = 0;
uint16_t seconds = 00;
uint16_t minutes = 00;
uint16_t hours = 12;
uint16_t alarmMinutes = 00;
uint16_t alarmHours = 11;
enum TIME_TO_SET {TIME_OF_DAY, ALARM_TIME};

//next alarm song, also last song selected
int currSongIndex = 0;

//something to keep track of UI entry time expiration
const uint16_t UI_TIMER_UNSET_OR_EXPIRED = 0;
const uint16_t MAX_UI_TIMER = 40; //40 deciseconds... 4 seconds
uint16_t uiTimer = UI_TIMER_UNSET_OR_EXPIRED;

//Is the alarm set to go off?
bool alarmSet = false;
//For BLINK_TIME state only, what is the UI setting?
TIME_TO_SET timeToSet = TIME_OF_DAY;

enum State {
  IDLE,
  TIME,
  SELECT_SONG,
  PLAY_SONG,
  SET_ALARM,//for editing ALARM on or OFF (alarmState).. NOT for setting ALARM_TIME
  BLINK_TIME, //for seeting TIME_OF_DAY or ALARM_TIME
  SHOW_MSG
} state = TIME;

void incrementTime() {
  //minimal time for us is a tenth of a second, for button debouncing
  ++deciseconds;

  //if the timer is in use, increment it
  if (uiTimer) {
    //if we increment the timesr from zero toward it's max and rollover
    if (!(++uiTimer %= MAX_UI_TIMER)) {
      //times up ..start over
      state = TIME;
    }
  }

  //maintain the time
  if (!((++deciseconds) %= 10)) {
    //every second
    if (state == IDLE) state = TIME;
    if (!((++seconds) %= 60)) {
      //every_minute
      if (!((++minutes) %= 60)) {
        //every hour
        if (!((++hours) %= 24)) {
          //every day
        }
      }
    }
  }
  //if we have reached the set alarm time, and the alarm is switched on, play the last song that was selected
  if ((hours == alarmHours) && (minutes == alarmMinutes) && (seconds == 0) && (alarmSet)) {
    state = PLAY_SONG;
  }
}

void restartUiTimer() {
  uiTimer = 1;
}

void setup() {
  Serial.begin(9600);

  //Timer stuff
  //initialize timer1, and set period to 1/10th of a second.. no idea why i have to double this
  Timer1.initialize(200000); 
  Timer1.attachInterrupt(incrementTime);
  setupDisplay();
  setupButtons();
  Serial.print("*** Nate's Clock, Build: ");
  Serial.print(__DATE__); Serial.print(" ");Serial.print(__TIME__); 
  Serial.println(" ***");
  state = TIME;
  sideScroll("hello");
  //playSong();
}

void printState() {
  switch (state) {
    case IDLE:
      if (!deciseconds && !seconds) {
        Serial.print(".");

      } break;
    case TIME: Serial.println("TIME"); break;
    case SELECT_SONG: Serial.println("SELECT_SONG"); break;
    case PLAY_SONG: Serial.println("PLAY_SONG"); break;
    case SET_ALARM: Serial.println("SET_ALARM"); break;
    case BLINK_TIME: Serial.println("BLINK_TIME"); break;
    case SHOW_MSG: Serial.println("SHOW_MSG"); break;
    default: Serial.print("OUT OF STATE! Value is ");Serial.println(state);break;
  }
}

//#define BUTTONS_ONLY

void loop() {
  //Button::print();
#ifndef BUTTONS_ONLY
  printState();
  if (state == TIME) {
    //b/c deciseconds interrupt periodicablly toggles TIME to IDLE
    state = IDLE;
    writeTime();
    setPanelLeds(alarmSet? ALARM_ON:ALARM_OFF);
    if (Button::isLHeld()) {
      write5("Alarm");
      restartUiTimer();
      state = SET_ALARM;
    }
    else if (Button::isRHeld()) {
      timeToSet = TIME_OF_DAY;
      restartUiTimer();
      state = BLINK_TIME;
    }
    else if (Button::isLPressed()) state = SELECT_SONG;
    else if (Button::isRPressed()) state = SHOW_MSG;
  }
  if (state == SET_ALARM){
      //give user a chance to get finger off button
      delay(20);
      //blink on or off indicators to hint to make a choice
      setPanelLeds((deciseconds%6 < 3)? LEFT:RIGHT);
      if (uiTimer > 15){
        //user has toggled the alarm on
        if (Button::isRPressed()){
          alarmSet = true;
          timeToSet = ALARM_TIME;
          restartUiTimer();
          state = BLINK_TIME;
        }
        if (Button::isLPressed()){
          alarmSet = false;
          restartUiTimer();
        }
      }
      //we also exit this state if the UiTimer times out
  }
  if (state == SELECT_SONG) {
    songsMenu();
    state = TIME;
  }
  if (state == PLAY_SONG) {
    playSong();
    state = TIME;
  }
  if (state == BLINK_TIME) {
    if (Button::isLPressed()) {
      (timeToSet == TIME_OF_DAY)? ++hours % 24:  ++alarmHours % 24;
      restartUiTimer();
    } else if (Button::isRPressed()) {
      (timeToSet == TIME_OF_DAY)? ++minutes % 24:  ++alarmMinutes % 24;
      restartUiTimer();
      //change the random seed arbitrarily when we set the time
      randomSeed(minutes);
    }
    (timeToSet == TIME_OF_DAY)? blinkTime(): blinkAlarmTime();
  }
  if (state == SHOW_MSG){
    Serial.println("crawl message");
    scrollMessage();
    state = TIME;
  }
#endif //BUTTONS_ONLY
}

void songsMenu() {
  //no double-tap (we got here by pressing a button, it may still be down)
  delay(20);
  //crawl song names until either a button is pushed, or we have no more songs
  for (int i = 0; i < NUM_SONGS; i++) {
    Serial.print("Scrolling song name: ");
    Serial.println(getSongName(i));
    if (sideScroll(getSongName(i))){
      currSongIndex = i;
      Serial.print("Song[");Serial.print(currSongIndex);Serial.println("] was selected");
      playSong();
      //instead of break{break;}
      goto done;
    }
  }
  done:;
}

void scrollMessage() {
  byte whichMsg = (byte)random(0,NUM_MESSAGES);

  //crawl song names until either a button is pushed, or we have no more songs
  sideScroll((byte *)messages[whichMsg]);
}

void playSong() {
  for (int thisNote = 0; thisNote < SONG_LEN; thisNote++) {
    const int pitch = getSongNoteFreq(currSongIndex, thisNote);
    // to calculate the {NOTE duration, take one second
    // divided by the {NOTE type.
    //e.g. quarter {NOTE = 1000 / 4, eighth {NOTE = 1000/8, etc.
    const int duration = 1000/ getSongNoteDuration(currSongIndex, thisNote);

    tone(SPKR_PIN, pitch, duration);
    Serial.print("tone Hz: ");Serial.print(pitch);Serial.print(" duration: ");Serial.println(duration);
    // to distinguish the {NOTEs, set a minimum time between them.
    // the {NOTE's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    //stop the tone playing;
    noTone(SPKR_PIN);
  }
  state = TIME;
}

void writeTime(int seconds, int minutes, int hours) {
  //display in regular not military time
  hours = ((hours - 1)%12)+1;
  char sz_time[6] = "     ";
  if (hours > 19) {
    sz_time[0] = '2';
  }
  else if (hours > 9) {
    sz_time[0] = '1';
  }
  else {
    sz_time[0] = ' ';
  }
  //fourthDigit
  sz_time[1] = getDigitAsChar(hours % 10);
  //thirdDigit ..blinking colon
  sz_time[2] = (seconds % 2 == 0) ? ':' : '-';
  //secondDigit
  sz_time[3] = getDigitAsChar((minutes - minutes % 10) / 10);
  //firstDigit
  sz_time[4] = getDigitAsChar(minutes % 10);
  Serial.println(sz_time);
  write5(sz_time);
}

void writeTime() {
  writeTime(seconds, minutes, hours);
}

void writeAlarmTime() {
  writeTime(0, alarmMinutes, alarmHours);
}

void blinkAlarmTime() {
  if ((deciseconds%10)>4)writeAlarmTime(); else write5("     ");
}


void blinkTime(){
  if ((deciseconds%10)>4)writeTime();else write5("     ");
}

//call this method on the Hertz change if you want to alternate tick tock
void writeTickTock() {
  if (tick == true) {
    write5("tick");
    tick = false;
  }
  else { /*(tick = false)*/
    write5("tock");
    tick = true;
  }
}

//convert a single digit to it's character value (eg 0 becomes '0')
char getDigitAsChar(int i) {
  //if its not a single digit.. just force it into one
  i = i % 10;

  //ASCII table starts 0-9 at 0x30
  char c = (char)i + 0x30;

  return c;
}
