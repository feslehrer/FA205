/*
 Beschreibung:
   
 Erstellt am: 
 Letzte �nderung:
 
 Autor: 
*/

#include "controller.h"

#define	SiebenSeg    _PORTD_
#define Start        _PORTB_,2
#define Speaker      _PORTB_,3

 uint8_t seg7code[] = { 0xc0,0xf9,0xa4,0xb0,
                        0x99,0x92,0x82,0xf8,
                        0x80,0x90 };

void setup (void)
{  /* Initialisierungen */
  byte_init(SiebenSeg,OUT);
  byte_write(SiebenSeg,0xff);     // Anzeige dunkelschalten
  byte_write(SiebenSeg,seg7code[0]); //Startwert 0
  
  bit_init(Start,IN);
  bit_init(Speaker,OUT);
}

// Funktion main()
void main(void)
{
  int8_t   n;
  uint16_t i;
  uint8_t  beep = 0x01;
  
  setup();

  while(1)                          // Endlosschleife loop()
  { 
    while(bit_read(Start)==1);   //Warten bis Taster bet�tigt
    for(n=9;n>=0;n--)
    {
      delay_ms(1000);
      byte_write(SiebenSeg,seg7code[n]);
    }
    
    for(i=0; i<2000; i++)
    {
      bit_write(Speaker,beep);
      delay_100us(5);
      beep=~beep;
    }
  }
}
