/*
 Beschreibung:
   
 Erstellt am:       
 Letzte Änderung:
 
 Autor: 
*/

#include "controller.h"

#define Ampel _PORTD_  //PortC mit Ampelwerte definieren Am PortC 8xLED  MEGA Pin30=C.7  Pin37=C.0

//-------------  VARIABLE DECLARATIONS GLOBAL-----------------------------


int8_t n;         	 //8 bit Variable 0...255 lokal definiert
uint8_t Ampelwerte[] = {0x49, 0x4b, 0x4c, 0x4a, 0x49, 0x59, 0x61, 0x51};	//Ampelwerte
int16_t zeit[] = {2000, 300, 1000, 300, 2000, 300, 1500, 300};  //Zeitwerte

void setup ( void )
{  // Initialisierungen
  byte_init(Ampel,OUT);
  // Pin 37 PORT C0, 36 PORT C1, 35 PC2, 34 PC3, 33 PC4, 32 PC5, 31 PC6, 30 PC7
  // Die Pins 37 bis 30 werden als Ausgang definiert
}

int main(void)
{
  setup();
  
  while (1)      // Endlosschleife loop()
  {
  for (n = 0; n < 8; ++n)		// Zähler 0 bis 7
  {
    byte_write(Ampel,Ampelwerte[n]);
    //Ampelwerte = Ampel [n];		 // auf die Werte vom array zugreifen
    delay_ms(zeit[n]);         	// Zeitverzögerung Arraywert Zeit x millisec
  }	//Ende FOR-Schleife          //delay_ms Fachvorschriften

  }
}

