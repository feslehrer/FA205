/*
 Beschreibung:
   
 Erstellt am:       
 Letzte Änderung:  16.12.18
 
 Autor: 
*/

#include "controller.h"

#define Ampel   _PORTC_

uint8_t Ampelwerte[] = {0x09, 0x0b, 0x0c, 0x0a, 0x09, 0x19, 0x21, 0x11};	//Ampelwerte
uint16_t zeit[]      = {2000,  800, 3000,  400, 1000,  800, 5000,  400};	//Zeitwerte
     
uint8_t phase=0;
volatile uint16_t count;

void setup(void)
{
  byte_init(Ampel,OUT);
     
  timer1ms_init( timer1ms_isr );    // Timer initialisieren
  timer1ms_enable();                // Timer freigeben
}

void main (void)                    //Beginn Hauptprogramm
{
  setup();
  while (1)                         //Beginn Endlosschleife
  {                        
    //nichts zu tun  !!!
  }
}

//************************************************************************
void timer1ms_isr(void)
{   
  count++;            // Diese Variable wird jedesmal um 1 erhöht ->jede msec
  byte_write(Ampel,Ampelwerte[phase]);

  if(count>=zeit[phase])  // Dieser Programmteil wird ausgeführt: 1msec x arraywert von zeit
  {
    count=0; phase++;
    if( phase==8 )   phase = 0;
  }
}
