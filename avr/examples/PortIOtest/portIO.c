/* 
  Beschreibung:     Testprogramm zur Umsetzung der Technischen Richtlinie FA205 für 
	                MyAVR-Board (ATMega8A)
                    Vorschlag für Bit- und Byte-IO
								
  Autor:            Rolf Rahm
  Datum:            26.05.2015
  Letzte Änderung:  12.02.2016
  Hinweise:									
*/
#include "controller.h"

#define	LED             &PORTB       // Alternativ: _PORTB_
#define	Taster          &PORTC       // Alternativ: _PORTC_

void setup (void)
{
	/* Initialisierungen */
  //byte_init(_PORTB,OUT);             // Portinitialisierung ist beim 8051 meistens nicht nötig
  //byte_init(_PORTD,IN);              // aber aus Kompatibilitätsgründen mit Technischer Richtlinie!

  bit_init(&PORTB,2,OUT);             // LED
  bit_init(LED,1,OUT);                // LED
  bit_init(LED,2,OUT);                // LED
  
  bit_init(Taster,0,IN);              // PortC.0 = Taster 1
  bit_init(Taster,1,IN);              // PortC.1 = Taster 2
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

    temp = bit_read(Taster,0);		
	temp = ~temp;
    bit_write(&PORTB,2,temp);         // drei Parameter !!!

	temp = bit_read(Taster,1);	
	temp = ~temp;
    bit_write(LED,2,temp);			
  }
}