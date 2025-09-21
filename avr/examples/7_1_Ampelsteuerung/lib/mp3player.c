/*
Programm:           Bibliothek für den DFPlayer Mini
Beschreibung:       Bibliothek verwendet den seriellen Empfangsinterrupt                    

Autor:              Rahm
Datum:			         1.03.2019
letzte Änderung:     1.04.2019
*/

#include "mp3player.h"
#include "interrupt.h"

#ifdef _LCD_H_
  const uint8_t char_ls[]      = {0x01,0x03,0x0F,0x0F,0x0F,0x0F,0x03,0x01};
  //const uint8_t char_pau[]     = {0x00,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x00};
  const uint8_t char_pau[]     = {0x00,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0x00};    
  //const uint8_t char_ply[]     = {0x08,0x0C,0x0E,0x0F,0x0E,0x0C,0x08,0x00};
  const uint8_t char_ply[]     = {0x00,0x08,0x0C,0x0E,0x0C,0x08,0x00,0x00};    
  //const uint8_t char_fwd[]     = {0x00,0x04,0x12,0x09,0x12,0x04,0x00,0x00};
  const uint8_t char_fwd[]     = {0x00,0x11,0x09,0x05,0x09,0x11,0x00,0x00};    
  //const uint8_t char_rwd[]     = {0x00,0x04,0x09,0x12,0x09,0x04,0x00,0x00};
  const uint8_t char_rwd[]     = {0x00,0x11,0x12,0x14,0x12,0x11,0x00,0x00};    
  const uint8_t char_stp[]     = {0x00,0x00,0x0E,0x0E,0x0E,0x00,0x00,0x00};
  const uint8_t char_repeat[]  = {0x00,0x00,0x0E,0x11,0x11,0x0E,0x00,0x00};
  const uint8_t char_random[]  = {0x00,0x00,0x1B,0x04,0x04,0x1B,0x00,0x00};
#endif

volatile uint8_t command;
volatile uint16_t reply;

void mp3_init(void)
{
  delay_ms(20);
#ifdef _LCD_H_
  lcd_init();
  
  lcd_defchar(char_pau,_C_PAUSE_);
  lcd_defchar(char_ply,_C_PLAY_);
  lcd_defchar(char_stp,_C_STOP_);
  lcd_defchar(char_ls, _C_LS_);
  lcd_defchar(char_repeat,_C_REPEAT_);
  lcd_defchar(char_fwd,_C_FWD_);
  lcd_defchar(char_rwd,_C_RWD_);
  lcd_defchar(char_random,_C_RANDOM_);
#endif

  mp3_reset();  // Reset Modul
  rs232_init();
  serial_interrupt_init(serial_interrupt_isr);   // Empfang von Statusmeldungen
  serial_interrupt_enable();
}

void mp3_reset(void)
{
  mp3_send_command(0x0C,0,0,0);  // Reset Modul
  delay_ms(3000);  
} 

void mp3_setVolume(uint8_t volume)
{
  mp3_send_command(0x06,0,0,volume);  // 
}

void mp3_increaseVolume(void)
{
  mp3_send_command(0x04,0,0,0);    //stop play
}

void mp3_decreaseVolume(void)
{
  mp3_send_command(0x05,0,0,0);    //stop play
}

void mp3_setRepeatPlay(uint8_t repeat)
{
  mp3_send_command(0x11,0,0,repeat & 0x01);    //start repeat play
}

void mp3_start(void)
{
  mp3_send_command(0x0d,0,0,0);    //start play
}

void mp3_stop(void)
{
  mp3_send_command(0x16,0,0,0);    //stop play
}

void mp3_sleep(void)
{
  mp3_send_command(0x0a,0,0,0);    //Modul schlafen schicken...
}

void mp3_pause(void)
{
  mp3_send_command(0x0e,0,0,0);    //Pause
}

void mp3_setEq(uint8_t eq)
{
  mp3_send_command(0x07,0,0,eq);    //stop play
} 

void mp3_prevTrack(void)
{
  mp3_send_command(0x02,0,0,0);    //previus track
}  

void mp3_nextTrack(void)
{
  mp3_send_command(0x01,0,0,0);    //next track
}

void mp3_playGlobalTrack(uint16_t track)        // 
{
  mp3_send_command(0x03,0,(uint8_t)(track>>8),(uint8_t)track);
}

void mp3_playMP3FolderTrack(uint16_t track)        //
{
  mp3_send_command(0x12,0,(uint8_t)(track>>8),(uint8_t)track);
}

void mp3_playAdvertisement(uint16_t track)        // 
{
  mp3_send_command(0x013,0,(uint8_t)(track>>8),(uint8_t)track);
}

void mp3_stopAdvertisement(void) 
{
  mp3_send_command(0x015,0,0,0);
}

/// folder: 01..99   ; track: 01...255
void mp3_playFolderTrack(uint8_t folder, uint8_t track)
{
    mp3_send_command(0x0f,0,folder,track);
}

void mp3_playRandomTrackFromAll(void)
{
  mp3_send_command(0x18,0,0,0);       // playback starten
}

void mp3_loopGlobalTrack(uint16_t track)
{
  mp3_send_command(0x08,0,(uint8_t)(track>>8),(uint8_t)track);       // playback starten
}

void mp3_setPlaybackSource(uint8_t source)
{
  mp3_send_command(0x09,0,0,source);  //
  delay_ms(2000);
}

void mp3_setPlaybackMode(uint8_t mode)
{
  mp3_send_command(0x08,0,0,mode);  //
}

// Querys 
uint8_t mp3_getPlaybackMode(void)
{
  uint16_t temp;
  
  mp3_send_command(0x45,0,0,0);       // Query Playback-Mode
  temp = mp3_waiting_for_reply(0x45);
  return temp;              // 0 = Repeat; 1 = FolderRepeat
}                           // 2 = Single Repeat; 3 = Random

uint8_t mp3_getVolume(void)
{
  uint16_t temp;
  
  mp3_send_command(0x43,0,0,0);       // Query current Track from SD
  temp = mp3_waiting_for_reply(0x43);
  return ((uint8_t) temp);
}

uint8_t mp3_getEq(void)
{
  uint16_t temp;
  
  mp3_send_command(0x44,0,0,0);       // Query current Track from SD
  temp = mp3_waiting_for_reply(0x44);
  return ((uint8_t) temp);
}

uint16_t mp3_getTotalTrackCount(void)
{
  uint16_t temp;
  
  mp3_send_command(0x48,0,0,0);       // Query the ttotal number of SD-Card Files
  temp = mp3_waiting_for_reply(0x48);
  return temp;
}

uint16_t mp3_getFolderTrackCount(void)
{
  uint16_t temp;
  
  mp3_send_command(0x4e,0,0,0);       // Query the ttotal number of SD-Card Files
  temp = mp3_waiting_for_reply(0x4e);
  return temp;
}

uint16_t mp3_getTotalFolderCount(void)
{
  uint16_t temp;
  
  mp3_send_command(0x4f,0,0,0);       // Query the ttotal number of SD-Card Files
  temp = mp3_waiting_for_reply(0x4f);
  return temp;
}

uint16_t mp3_getCurrentTrack(void)
{
  uint16_t temp;
  
  mp3_send_command(0x4b,0,0,0);       // Query current Track from SD
  temp = mp3_waiting_for_reply(0x4b);
  return temp;
}

uint16_t mp3_getStatus(void)
{
  uint16_t temp;
  
  mp3_send_command(0x42,0,0,0);       // 
  temp = mp3_waiting_for_reply(0x42);
  return temp;
}

//  Low-Level Funktionen
void mp3_send_command(uint8_t command, uint8_t feedback, uint8_t p_msb, uint8_t p_lsb)
{
  int16_t checksum = 0;
  uint8_t command_buf[10];
  
  command_buf[0] = 0x7e;
  command_buf[1] = 0xff;
  command_buf[2] = 0x06;
  command_buf[3] = command;
  command_buf[4] = feedback;
  command_buf[5] = p_msb;
  command_buf[6] = p_lsb;
  
  for (uint8_t i = 1; i<7; i++)
  {
    checksum -= command_buf[i];
  }
  
  command_buf[7] = (uint8_t) (checksum>>8);
  command_buf[8] = (uint8_t) (checksum & 0x00ff);
  command_buf[9] = 0xef;
  
  for (uint8_t i = 0; i<10; i++)
  {
    rs232_put(command_buf[i]);
  }
  delay_ms(300);
}

uint8_t mp3_receive_query( uint8_t *command, uint16_t *argument)
{
  volatile uint8_t buffer[10];         // Puffer für Rückgabestring vom Player
  volatile uint16_t timeout = 0;
  volatile int16_t checksum;
  volatile uint8_t len=0;
  volatile uint16_t temp;

  *command = 0; *argument = 0;        // Rückgabewerte initialisieren

  do
  {
    len = rs232_readbytes(&buffer[0],1);    // Startbyte (0x7E) lesen
    if (len != 1) return 0;    // Fehler
  }  while ( buffer[0] != 0x7E);           // Startbyte lesen
  
  len += rs232_readbytes(&buffer[1],9);    // restliche 9 Bytes lesen
  // Test auf ungültige Daten
  if (len < 10)  return 0;
  if (buffer[1] != 0xFF || buffer[2] != 0x06 || buffer[9] != 0xef )   return 0;
  
  // checksum berechnen
  checksum = 0;
  for (uint8_t i = 1; i <= 6; i++)  checksum -= buffer[i];
  //checksum test
  temp = ( buffer[7] << 8) | buffer[8];
  if ( checksum != temp)   return 0;
  
  *command = buffer[3];
  *argument = ((uint16_t)buffer[5] << 8) | buffer[6];
  return 1;
}

uint16_t mp3_waiting_for_reply(uint8_t reply_command)
{
  uint16_t timeout;
  
  while (command != reply_command)
  {
    if(timeout>=0xfffe) return 0;
    else                 timeout++;
  }
  
  return reply;
}

void serial_interrupt_isr(void)
{
  uint8_t err = 0;
  
  err = 0;
  err = mp3_receive_query( &command, &reply );
  
  lcd_setcursor(2,1);
  if (err == 0)
  {
    OnEvent = _ERROR_;
    return;
  }

  switch (command)
  {
    case 0x3a:  // SD-Card inserted
                OnEvent = _CARD_INSERTED_;
                break;
    case 0x3b:  // SD-CARD removed
                OnEvent = _CARD_REMOVED_;
                break;
    case 0x3d:  // Track Finished
                OnEvent = _TRACK_FINISHED_;
    case 0x3f:  // Initialization Ready                
                if (reply == 0x02) OnEvent = _INITIALIZED_;
                break;
    case 0x40:  // Error
                OnEvent = _ERROR_;
                break;
  }
}
