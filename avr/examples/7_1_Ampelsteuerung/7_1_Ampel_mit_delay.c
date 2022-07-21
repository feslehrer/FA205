/*
 Beschreibung:      Zeitgesteuerte Verkehrsampel mit Software-Delay
   
 Erstellt am:       5.2.17 
 Letzte Änderung:  16.12.18
 
 Autor:             Rahm
*/

#include "controller.h"

#define Ampel   _PORTC_

uint8_t phase;         	       //8 bit Variable 0...255 lokal definiert
uint8_t Ampelwerte[] = {0x09, 0x0b, 0x0c, 0x0a, 0x09, 0x19, 0x21, 0x11};	//Ampelwerte
uint16_t zeit[]      = {2000,  800, 3000,  400, 1000,  800, 5000,  400};	//Zeitwerte

void setup ( void )
{  // Initialisierungen
  byte_init(Ampel,OUT);
}

int main(void)
{
  setup();
  
  while (1)                 // Endlosschleife loop()
  {
    for (phase = 0; phase< 8; ++phase)		// Zähler 0 bis 7
    {
      byte_write(Ampel,Ampelwerte[phase]);
      delay_ms(zeit[phase]);
    }
  }
}

