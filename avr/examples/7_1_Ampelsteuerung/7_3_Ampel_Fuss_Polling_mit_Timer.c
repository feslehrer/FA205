/*
 Beschreibung:
   
 Erstellt am:       
 Letzte Änderung:
 
 Autor: 
*/

#include "controller.h"

#define Ampel   _PORTC_
#define Fuss    _PORTB_
#define gruen   2
#define rot     3
#define On      1
#define Off     0
#define Taster  _PORTD_
#define PD2      2

uint8_t Ampelwerte[] = {0x09, 0x0b, 0x0c, 0x0a, 0x09, 0x19, 0x21, 0x11};	//Ampelwerte
uint16_t zeit[]      = {2000,  400, 3000,  400, 1000,  400, 5000,  400};	//Zeitwerte
     
volatile uint8_t phase=0;
volatile uint16_t count;
volatile uint8_t status = 0;

void setup(void)
{
  byte_init(Ampel,OUT);
  bit_init(Fuss,gruen,OUT);
  bit_init(Fuss,rot,OUT);
  bit_write(Fuss,gruen,Off);
  bit_write(Fuss,rot,On);           // Fussgängerampel rot
  bit_init(Taster,PD2,IN);
     
  timer1ms_init( timer1ms_isr );    // Timer initialisieren
  timer1ms_enable();                // Timer freigeben
}

void main (void)                    // Beginn Hauptprogramm
{
  setup();
  while (1)                         // Beginn Endlosschleife
  {
    bit_toggle(Taster,PD2,&status);
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
    if( phase==8 )
    {
      if( status )            //Wenn Interrupt aufgetreten und Zyklus durchgelaufen!
      {
        byte_write(Ampel,0x09);        //Ampel auf rot
        //bit_write(Fuss,gruen,Off);
        //bit_write(Fuss,rot,On);         // Fussgängerampel rot
        delay_ms(1000);
        bit_write(Fuss,gruen,On);
        bit_write(Fuss,rot,Off);         // Fussgängerampel grün
        delay_ms(5000);
        bit_write(Fuss,gruen,Off);
        bit_write(Fuss,rot,On);         // Fussgängerampel rot
        delay_ms(2000);
        status = 0;
      }
      phase = 0;      
    }
  }
}
