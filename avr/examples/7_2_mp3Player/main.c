/*
Programm:
Beschreibung:

Autor:
Datum:			        24.04.2017
letzte Änderung:
*/

#include "controller.h"
#include "mp3player.h"

//#define PLAY_STATE _PORTD_
//#define BUSY         2

#define Taster _PORTB_
#define Playback    2
#define Rwd         3
#define Play        4
#define Fwd         5

uint16_t current_track_number = 1;
uint8_t current_folder = 1;
uint16_t total_tracks_number = 0;

volatile uint16_t milli = 0;
volatile uint8_t sek = 0;
volatile uint8_t min = 0;

void setup (void)   // Initialisierungen
{
  rs232_init();
  adc_init();
  lcd_init();
  lcd_clear();
  
  //bit_init(PLAY_STATE,BUSY,IN);   // Wiedergabe = 1; Stop = 0
  bit_init(Taster,Rwd,IN);
  bit_init(Taster,Play,IN);
  bit_init(Taster,Fwd,IN);
  bit_init(Taster,Playback,IN);
  
  lcd_setcursor(1,1);
  lcd_print("MP3-Player");
  lcd_setcursor(1,13); lcd_char(_C_RANDOM_);
  lcd_setcursor(2,13);
  lcd_char(_C_STOP_);lcd_char(_C_RWD_);lcd_char(_C_PLAY_);lcd_char(_C_FWD_);  
  
  mp3_init();
  total_tracks_number = mp3_getTotalTrackCount();
  lcd_setcursor(2,1);
  lcd_print("Total:"); lcd_int(total_tracks_number);
  delay_ms(1000);
  mp3_setPlaybackSource(1);
  timer1ms_init(timer1ms_isr);  // Für genaue Track-Time Angabe
  mp3_setVolume(2);             // Silence !!
}

int main (void)
{
  uint16_t volume;
  
  setup(); 
    
  mp3_playRandomTrackFromAll();   // Alle Tracks zufällig spielen
  //mp3_loopGlobalTrack(24);        // Track 24 endlos spielen
  //mp3_setRepeatPlay(1);           // Aktuellen Track endlos spielen
  mp3_stop();                     // Wiedergabe anhalten
  
  playback_mode = _PLAYBACK_RANDOM_;
  play_mode     = _PLAYBACK_STOP_;
  mp3_setEq(_EQ_ROCK_);           // Klang = Rock

  while(1)          // loop()
  {
    volume = adc_in1();
    volume *= 10;
    volume /= 85;
    mp3_setVolume(volume);
    //volume = mp3_getVolume();   // Einstellung über Taster auf Shield
    lcd_setcursor(1,14);
    lcd_char(_C_LS_); lcd_dd(volume);
  
    if (!bit_read(Taster,Play))
    {
      switch (play_mode)
      {
        case _PLAYBACK_STOP_:
                play_mode = _PLAYBACK_PLAY_;
                milli = 0; sek = 0; min = 0;
                timer1ms_enable();
                lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
                mp3_start();
                break;
        case _PLAYBACK_PLAY_:
                play_mode = _PLAYBACK_PAUSE_;
                timer1ms_disable();
                lcd_setcursor(2,15); lcd_char(_C_PLAY_); 
                mp3_pause();
                break;
        case _PLAYBACK_PAUSE_:
                play_mode = _PLAYBACK_PLAY_;
                timer1ms_enable();
                mp3_start();
                lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
                break;
      }
    }
    
    if (!bit_read(Taster,Fwd))
    {                
      mp3_nextTrack();
      timer1ms_enable();
      milli = 0; sek = 0; min = 0;
      play_mode = _PLAYBACK_PLAY_;
      lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
    }
    
    if(!bit_read(Taster,Rwd))
    {
      mp3_prevTrack();
      timer1ms_enable();
      milli = 0; sek = 0; min = 0;
      play_mode = _PLAYBACK_PLAY_;
      lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
    }
    
    if(!bit_read(Taster,Playback))
    {
      mp3_stop();
      timer1ms_disable();
      milli = 0; sek = 0; min = 0;
      play_mode = _PLAYBACK_STOP_;
      lcd_setcursor(2,13); lcd_char(_C_STOP_);      
      lcd_setcursor(2,15); lcd_char(_C_PLAY_); 
    }
    
    if(play_mode == _PLAYBACK_PLAY_)
    {
      current_track_number = mp3_getCurrentTrack();
      lcd_setcursor(2,7); lcd_int(current_track_number);
    }
    
    switch (OnEvent)
    {
      case _TRACK_FINISHED_:
      
                    if (playback_mode == _PLAYBACK_RANDOM_)
                    {
                       milli = 0; sek = 0; min = 0;
                       mp3_nextTrack();
                    }
                    else
                    {
                       play_mode = _PLAYBACK_STOP_;                   
                       timer1ms_disable();
                       lcd_setcursor(2,15); lcd_char(_C_PLAY_);
                    }

                    current_track_number = mp3_getCurrentTrack();
                    lcd_setcursor(2,7); lcd_int(current_track_number);
                    break;
      case _CARD_REMOVED_:
                    play_mode = _PLAYBACK_STOP_;
                    lcd_setcursor(2,1); lcd_print("SD removed  ");
                    lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;
      case _CARD_INSERTED_:
                    play_mode = _PLAYBACK_STOP_;
                    lcd_setcursor(2,1); lcd_print("SD inserted ");                    
                    lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;      
      case _INITIALIZED_:
                    play_mode = _PLAYBACK_STOP_;
                    lcd_setcursor(2,1); lcd_print("initialized ");
                    lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;
      case _ERROR_:
                    play_mode = _PLAYBACK_STOP_;
                    lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;     
      case _NONE_:
      default:      break;
    }
    OnEvent = _NONE_;
    
    lcd_setcursor(2,1);
    lcd_dd(min);lcd_char(':');lcd_dd(sek);lcd_char(' ');
  }
}

void timer1ms_isr(void)
{
  milli++;
  if (milli > 999)
  {
    sek++; milli = 0;
    if (sek > 59)
    {
      min++; sek = 0;
      if (min > 59)
      min = 0;
    }
  }
}

