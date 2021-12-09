// Technische Richtlinie FA205
// Bibliothek:       delay.c
// Controller:       Atmel ATmega8, ATmega328
// Version:          2.0
// erstellt am:      28.10.2014
// letzte Änderung:  9.4.2018
// Autor:            Rahm
#include "delay.h"
#include <util/delay.h>

void delay_us( uint16_t mikrosekunden)
{
    _delay_us(mikrosekunden);
}

void delay_100us( uint16_t mikrosekunden)
{
	uint16_t n = 0;

	while(n < mikrosekunden)
	{
		_delay_us(100);
		n++;
	}
}

void delay_ms( uint16_t millisekunden)
{
	uint16_t n = 0;
	
	while(n < millisekunden)
	{
		delay_100us(10);
		n++;
	}
}

void delay_s( uint16_t sekunden)
{
  uint16_t n = 0;
  
  while(n < sekunden)
  {
    _delay_ms(1000);
    n++;
  }
}