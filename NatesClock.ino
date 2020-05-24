//use Sketch/IncludeLibrary/ManageLibraries to install TimerOne
//TimeOne: https://playground.arduino.cc/Code/Timer1/
#include "TimerOne.h"
#include "pitches.h"
#include "nates_symbols.h"
#include "messages.h"
#include "songs.h"
#include "display.h"
#include "dbButton.h"

bool tick = true;
const char sz_tick[] = "tick ";
const char sz_tock[] = "tock ";
int deciseconds = 0;
int seconds = 00;
int minutes = 00;
int hours = 12;
int alarmMinutes = 00;
int alarmHours = 12;
boolean alarmState = false;
enum TIME_TO_SET {TIME_OF_DAY, ALARM_TIME};


//next alarm song, also last song selected
int currSongIndex = 0;

//something to keep track of UI entry time expiration
const int UI_TIMER_UNSET_OR_EXPIRED = 0;
const int MAX_UI_TIMER = 40; //40 deciseconds... 4 seconds
int uiTimer = UI_TIMER_UNSET_OR_EXPIRED;

//Is the alarm set to go off?
bool alarmSet = false;
//For BLINK_TIME state only, what is the UI setting?
TIME_TO_SET timeToSet = TIME_OF_DAY;

enum State {
  TIME,
  CRAWL_SONGS,
  PLAY_SONG,
  SET_ALARM,
  BLINK_TIME,
  CRAWL_MSG
} state = TIME;

//oops, I think these are technically analog pins :}
Button leftButton(19), rightButton(18);

void incrementTime() {
  //minimal time for us is a tenth of a second, for button debouncing
  deciseconds++;

  //if the timer is in use, increment it
  if (uiTimer != UI_TIMER_UNSET_OR_EXPIRED) {
    //increment the timer from zero toward it's max
    ++uiTimer %= MAX_UI_TIMER;
    //if the UI Timer expires go back to showing the time
    if (uiTimer = UI_TIMER_UNSET_OR_EXPIRED) {
      state = TIME;
    }
  }

  leftButton.debounce();
  rightButton.debounce();

  if (deciseconds == 10) {
    deciseconds = 0;
    seconds ++;
    if (seconds == 60) {
      minutes = minutes ++;
      seconds = 0;
      if (minutes == 60) {
        hours += 1;
        minutes = 0;
        if (hours == 25) {
          hours = 1;
        }
      }
    }
  }
  //if we have reached the set alarm time, play the last song that was selected
  if ((hours == alarmHours) && (minutes == alarmMinutes) && (seconds == 0) && (state != PLAY_SONG)){
    state = PLAY_SONG;
  }
}

void restartUiTimer() {
  uiTimer = 1;
}

void setup() {
  //Timer stuff
  Timer1.initialize(100000); //initialize timer1, and set period to 1/10th of a second
  Timer1.attachInterrupt(incrementTime);
  setupDisplay();
}

void loop() {
  if (state == TIME) {
    writeTime();
    if (leftButton.isHeld()) state = SET_ALARM;
    else if (rightButton.isHeld()) {
      timeToSet = TIME_OF_DAY;
      restartUiTimer();
      state = BLINK_TIME;
    }
    else if (leftButton.isPressed()) state = CRAWL_SONGS;
    else if (rightButton.isPressed()) state = CRAWL_MSG;
  }
  if (state == CRAWL_SONGS) {
    //crawl_songs
    state == PLAY_SONG;
  }
  if (state == PLAY_SONG) {
    playSong();
    //after song, go back to playing time
    state = TIME;
  }
  if (state == SET_ALARM) {
    timeToSet = ALARM_TIME;
    //give user some time to do somethign about the blinking
    restartUiTimer();
    state = BLINK_TIME;
  }
  if (state == BLINK_TIME) {
    if (leftButton.isPressed()) {
    if (timeToSet == TIME_OF_DAY) ++hours % 24; else ++alarmHours % 24;
      restartUiTimer();
    } else if (rightButton.isPressed()) {
    if (timeToSet == TIME_OF_DAY) ++minutes % 24; else ++alarmMinutes % 24;
      restartUiTimer();
    }
  }
}

int crawlSongs() {
  //crawl song names until either a button is pushed, or we have no more songs
  for (int i = 0; i < NUM_SONGS; i++) {
    writeCreeper((byte *)getSongName(i), getSongLength(i), 1);
  }
}

void playSong() {
  for (int thisNote = 0; thisNote < NUM_SONGS; thisNote++) {
    const int pitch = getSongNoteFreq(currSongIndex, thisNote);
    // to calculate the {NOTE duration, take one second
    // divided by the {NOTE type.
    //e.g. quarter {NOTE = 1000 / 4, eighth {NOTE = 1000/8, etc.
    const int duration = getSongNoteDuration(currSongIndex, thisNote)/1000;

    tone(3, pitch, duration);
    // to distinguish the {NOTEs, set a minimum time between them.
    // the {NOTE's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    //stop the tone playing;
    noTone(3);
  }
  state = TIME;
}

void writeTime(int seconds, int minutes, int hours) {
  char sz_time[6] = "     ";
  if (hours > 19){
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
  write5(sz_time);
}

void writeTime() {
  writeTime(seconds, minutes, hours);
}

void writeAlarmTime() {
  writeTime(0, alarmMinutes, alarmHours);
}

void blinkAlarmTime() {
  if (deciseconds > 4) writeAlarmTime(); else write5("     ");
}

void blinkTime() {
  if (deciseconds > 4) writeTime(); else write5("     ");
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
char getDigitAsChar(int i){
  //if its not a single digit.. just force it into one
  i = i%10;
  
  //ASCII table starts 0-9 at 0x30
  char c = (char)i + 0x30;
  
  return c;
}






