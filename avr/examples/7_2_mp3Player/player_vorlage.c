/*
Programm:
Beschreibung:

Autor:
Datum:			        24.04.2017
letzte Änderung:
*/

#include "controller.h"
#include "mp3player.h"

#define PLAY_STATE _PORTD_
#define BUSY         2

#define Taster _PORTB_
#define Playback    2
#define Rwd         3
#define Play        4
#define Fwd         5

uint16_t current_track_number = 1;
uint8_t  current_folder = 1;
uint16_t total_tracks_number = 0;

void setup (void)   // Initialisierungen
{
  rs232_init();  
  mp3_init();
  total_tracks_number = mp3_getTotalTrackCount();
  delay_ms(1000);
  mp3_setEq(_EQ_ROCK_);
  mp3_setPlaybackSource(1);
  mp3_setVolume(2);             // Silence !!
}

int main (void)
{
  uint16_t volume;
  
  setup();
  
  //mp3_playRandomTrackFromAll();
  //mp3_loopGlobalTrack(24);
  //mp3_setRepeatPlay(1);

  while(1)          // loop()
  {    
    
    
    
    switch (OnEvent)
    {
      case _TRACK_FINISHED_:   break;
      case _CARD_REMOVED_:     break;
      case _CARD_INSERTED_:    break;
      case _INITIALIZED_:      break;
      case _ERROR_:            break;
      case _NONE_:             break;
    }
    OnEvent = _NONE_;
  }
}


