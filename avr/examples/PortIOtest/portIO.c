/* 
  Beschreibung:   Testprogramm zur Umsetzung der Technischen Richtlinie FA205 
                  für ATMega328P auf ARduino-Carrier-Board
                  Vorschlag für Bit- und Byte-IO
								
  Autor:            Rolf Rahm
  Datum:            26.05.2015
  Letzte Änderung:   9.11.2024
  Hinweise:									
*/
#include "controller.h"

#define	LED1         _PORTD_,1
#define	LED2         _PORTD_,2

#define	S1           _PORTB_,2
#define	S2           _PORTB_,3

void setup (void)
{
	/* Initialisierungen */
  bit_init(LED1,OUT);           // PD1
  bit_init(LED2,OUT);           // PD2
  
  bit_init(S1,IN);              // PB2
  bit_init(S2,IN);              // PB3
}

// Funktion main()
int main(void)
{
  setup();

  while(1)                          // Endlosschleife loop()
  {	
    uint8_t temp;

    // Byte-Operationen
    //temp = byte_read(_PORTD);
    //byte_write(_PORTB,temp);

    // Bit-Operationen:
    // mit Funktionen ...

    temp = bit_read(S1);		
	  temp = ~temp;
    bit_write(LED1,temp);          
    // Alternativ mit 3 Parametern !!!
    // bit_write(_PORTD_,1,temp);   //Port, Bit, Wert
	  temp = bit_read(S2);	
	  temp = ~temp;
    bit_write(LED2,temp);			
  }
}