/*
 * srf04.c
 *
 * Created: 07.03.2016 17:20:04
 *  Author: Rolf
 */
#include "srf04.h"

uint8_t teiler = 0x03;      // Vorteiler = 64
//1:		teiler = 0x01	==> t[µs] = Timerwert * 1 / F_CPU / 1000000
//8:		teiler = 0x02   ==> t[µs] = Timerwert * 8 / F_CPU / 1000000
//64:		teiler = 0x03   ==> t[µs] = Timerwert * 64 / F_CPU / 1000000
//256:		teiler = 0x04   ==> t[µs] = Timerwert * 256 / F_CPU / 1000000
//1024:	    teiler = 0x05   ==> t[µs] = Timerwert * 1024 / F_CPU / 1000000

void srf04_start( void )
{
  TCNT1  = 0x0000;
  TCCR1B = teiler;          // 16 Bit-Timer mit Vorteiler 64 starten
}

uint32_t srf04_stop( void )
{
  volatile uint32_t usec;

  TCCR1B = 0x00;	// 16 Bit-Timer stoppen

  usec = 64 / (F_CPU / 1000000UL);
  usec *=  TCNT1;

  return usec;	    // Gibt die Echo-Zeit in µs zurück
}
