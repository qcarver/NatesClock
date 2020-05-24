#ifndef NATESSONGSH
#define NATSESONGSH

#include "pitches.h"

const uint8_t Songs[]=
    {
    //"Blue Bird Song",
    NOTE_E5, 8, NOTE_GS5, 8, NOTE_GS5, 8, NOTE_B5, 8,  NOTE_CS6, 8,  NOTE_B5, 8,  NOTE_B5, 4,  NOTE_E6, 8,  
    NOTE_B5, 8, NOTE_B5, 8,  NOTE_GS5, 8, NOTE_GS5, 8, NOTE_FS5, 16, NOTE_E5, 16, NOTE_FS5, 8, NOTE_B4, 8,
    NOTE_A5, 8, NOTE_A5, 8,  NOTE_GS5, 8, NOTE_GS5, 8, NOTE_FS5, 4,  NOTE_E5, 8,  NOTE_GS5, 16, NOTE_A5, 16,
    NOTE_B5, 8, NOTE_B5, 8,  NOTE_B5, 8,  NOTE_A5, 8,  NOTE_GS5, 8,  NOTE_FS5, 8, NOTE_E5, 4
    ,
    //"Good Morning To You", 
    NOTE_F4, 16,  NOTE_F4, 8, NOTE_E5, 8,  NOTE_G5, 8, NOTE_C6, 4, NOTE_F4, 16,  NOTE_F4, 8, NOTE_E5, 8, 
    NOTE_G5, 8, NOTE_C6, 4, NOTE_C6, 8, NOTE_C6, 8, NOTE_G5, 8, NOTE_G5, 8, NOTE_G5, 8, NOTE_E5, 8, 
    NOTE_C6, 8, NOTE_C6, 8, NOTE_G5, 8, NOTE_G5, 8, NOTE_G5, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_F4, 8,
    NOTE_E5, 8, NOTE_G5, 8, NOTE_C6, 4, NOTE_C6, 8, NOTE_C6, 8, NOTE_G5, 8, NOTE_E5, 8, NOTE_F4, 4
    };

//TODO: dirt simple for now... for 2 songs.. don't get crazy
#define NUM_SONGS 2
#define SONG1_OFFSET 0
#define SONG2_OFFSET 31
#define SONG1_LEN 31
#define SONG2_LEN 32

const char & getSongName(uint8_t index){
  return (!index)? "Blue Bird Song" : "Good Morning To You";
}

const char & getSongLength(uint8_t index){
  return (!index)? SONG1_LEN: SONG2_LEN;
}

int getSongNoteFreq(uint8_t songIndex, uint8_t noteIndex){
  return getEnumHertz(Songs[(!songIndex)? noteIndex * 2: SONG1_OFFSET + noteIndex * 2]);
}

int getSongNoteDuration(uint8_t songIndex, uint8_t noteIndex){
  return Songs[((!songIndex)? noteIndex * 2: SONG1_OFFSET + noteIndex * 2)+1];

}

#endif
