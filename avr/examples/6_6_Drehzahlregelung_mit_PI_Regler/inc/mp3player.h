/*
Programm:
Beschreibung:       Header-Datei für MP3-Player mit DFPlayer Mini - Modul

Autor:              Rahm
Datum:			        23.02.2019
letzte Änderung:    1.4.19
*/

#ifndef _MP3PLAYER_H_
#define _MP3PLAYER_H_

#include "controller.h"

// Zeichendefinitionen
#define _C_LS_              0
#define _C_STOP_            1
#define _C_PLAY_            2
#define _C_PAUSE_           3
#define _C_FWD_             4
#define _C_RWD_             5
#define _C_REPEAT_          6
#define _C_RANDOM_          7

enum
{
  _PLAYBACK_REPEAT_        = 0,
  _PLAYBACK_FOLDER_REPEAT_,
  _PLAYBACK_SINGLE_REPEAT_,
  _PLAYBACK_RANDOM_,
} playback_mode;

enum
{
  _PLAYBACK_STOP_          = 0,
  _PLAYBACK_PLAY_,
  _PLAYBACK_PAUSE_
} play_mode;

enum EQ_MODE
{
  _EQ_NORMAL_    = 0,
  _EQ_POP_,
  _EQ_ROCK_,
  _EQ_JAZZ_,
  _EQ_CLASSIC_,
  _EQ_BASS_,
};

//Globale Ereignisse
enum
{
  _NONE_         = 0,
  _TRACK_FINISHED_,
  _CARD_REMOVED_,
  _CARD_INSERTED_,
  _INITIALIZED_,
  _ERROR_             
} OnEvent;

// Initialisierung-, Status- und Info-Funktionen
void mp3_init(void);
void mp3_reset(void);
void mp3_sleep(void);
uint16_t mp3_getStatus(void);
uint8_t  mp3_getPlaybackMode(void);    //
uint16_t mp3_getTotalTrackCount(void);
uint16_t mp3_getFolderTrackCount(void);
uint16_t mp3_getTotalFolderCount(void);
uint16_t mp3_getCurrentTrack(void);

// Lautstärke, EQ
void     mp3_setVolume(uint8_t volume);
uint8_t  mp3_getVolume(void);
void     mp3_increaseVolume(void);
void     mp3_decreaseVolume(void);
void     mp3_setEq(uint8_t eq);
uint8_t  mp3_getEq(void);

// Abspiel-Steuerung
void mp3_playGlobalTrack(uint16_t track);
void mp3_start(void);
void mp3_stop(void);
void mp3_pause(void);
void mp3_prevTrack(void);
void mp3_nextTrack(void);
void mp3_setRepeatPlay(uint8_t repeat);  //1=start; 0=stop
void mp3_playRandomTrackFromAll(void);
void mp3_loopGlobalTrack(uint16_t track);
void mp3_playFolderTrack(uint8_t folder, uint8_t track);
void mp3_playMP3FolderTrack(uint16_t track);
void mp3_setPlaybackSource(uint8_t source);
void mp3_setPlaybackMode(uint8_t mode);
void mp3_playAdvertisement(uint16_t track);
void mp3_stopAdvertisement(void);

// Low-Level-Funktionen
void     mp3_send_command(uint8_t command, uint8_t feedback, uint8_t p_msb, uint8_t p_lsb);
uint8_t  mp3_receive_query( uint8_t *command, uint16_t * argument);
uint16_t mp3_waiting_for_reply(uint8_t reply_command);

#endif