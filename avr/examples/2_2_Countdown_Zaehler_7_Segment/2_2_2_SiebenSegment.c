/*
 Beschreibung:
   
 Erstellt am: 
 Letzte Änderung:
 
 Autor: 
*/

#include "controller.h"

#define	SiebenSeg    _PORTD_
#define Taster       _PORTB_
#define Start        2

const uint8_t seg7code[] = { 0xc0,0xf9,0xa4,0xb0,
                             0x99,0x92,0x82,0xf8,
                             0x80,0x90 };

void setup (void)
{  /* Initialisierungen */
  byte_init(SiebenSeg,OUT);
  byte_write(SiebenSeg,0xff);     // Anzeige dunkelschalten
  bit_init(Taster,Start,IN);
}

// Funktion main()
void main(void)
{
  volatile int8_t n;
  
  setup();

  while(1)                          // Endlosschleife loop()
  { 
    while(bit_read(Taster,Start)==1);   //Warten bis Taster betätigt
    for(n=9;n>=0;n--)
    {
      byte_write(SiebenSeg,seg7code[n]);
      delay_ms(1000);
    }
  }
}