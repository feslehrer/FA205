/*
 Beschreibung:
 
 Erstellt am: 
 Letzte �nderung:
 
 Autor: 
*/

#include "controller.h"

#define	SiebenSegment   _PORTD_
#define Start           _PORTB_,2

void setup (void)
{  /* Initialisierungen */
  byte_init(SiebenSegment,OUT);
  byte_write(SiebenSegment,0xff);     // Anzeige dunkelschalten

  bit_init(Start,IN);
}

// Funktion main()
void main(void)
{
  setup();

  while(1)                          // Endlosschleife loop()
  {
    while(bit_read(Start)==1);      //Warten bis Taster betötigt

    byte_write(SiebenSegment,0x90);
    delay_ms(1000);
    byte_write(SiebenSegment,0x80);
    delay_ms(1000);
    byte_write(SiebenSegment,0xf8);
    delay_ms(1000);
    byte_write(SiebenSegment,0x82);
    delay_ms(1000);
    byte_write(SiebenSegment,0x92);
    delay_ms(1000);
    byte_write(SiebenSegment,0x99);
    delay_ms(1000);
    byte_write(SiebenSegment,0xb0);
    delay_ms(1000);
    byte_write(SiebenSegment,0xa4);
    delay_ms(1000);
    byte_write(SiebenSegment,0xf9);
    delay_ms(1000);
    byte_write(SiebenSegment,0xc0);
    delay_ms(1000);
  }
}