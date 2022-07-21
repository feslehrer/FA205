/*
 Beschreibung:      (Fußgänger-)ampel mit Timer- und Interruptsteuerung
   
 Erstellt am:       5.2.17
 Letzte Änderung:	16.12.18
 
 Autor:           Rahm
*/

#include "controller.h"

#define Ampel   _PORTC_
#define Fuss    _PORTB_
#define gruen   2
#define rot     3

#define On      1
#define Off     0

uint8_t Ampelwerte[] = {0x09, 0x0b, 0x0c, 0x0a, 0x09, 0x19, 0x21, 0x11};	//Ampelwerte
uint16_t zeit[]      = {2000,  800, 3000,  400, 1000,  800, 5000,  400};	//Zeitwerte
	
volatile uint8_t phase=0, int_status;       //volatile, sonst werden die Variablen rausoptimiert???
uint16_t count=0;

void setup(void)
{
  byte_init(Ampel,OUT);
  bit_init(Fuss,gruen,OUT);
  bit_init(Fuss,rot,OUT);         //   
  bit_init(_PORTD_,2,IN);		  // Pull-Up für Interrupteingang aktivieren!
								
  bit_write(Fuss,gruen,Off);
  bit_write(Fuss,rot,On);         // Fussgängerampel rot

  timer1ms_init( timer1ms_isr );
  timer1ms_enable(); 
  ext_interrupt_init( ext_interrupt_isr );
  ext_interrupt_enable();
}

//********************************************************************
void main (void)        //Beginn Hauptprogramm
{
  setup();
  while (1)                          //endlos
  {                        
    if( int_status & !phase )        //Wenn Interrupt aufgetreten und Zyklus durchgelaufen!
    {
      timer1ms_disable();            //Timer sperren sonst läuft Ampel normal weiter
      int_status = 0;
      byte_write(Ampel,0x09);        //Ampel auf rot
      delay_ms(2000);
      bit_write(Fuss,gruen,On);
      bit_write(Fuss,rot,Off);       // Fussgängerampel grün
      delay_ms(6000);
      bit_write(Fuss,gruen,Off);
      bit_write(Fuss,rot,On);        // Fussgängerampel rot
      delay_ms(2000);
      timer1ms_enable();
    }
  }
}
 
//***********************************************************************
void timer1ms_isr(void)
{   
   count++;            // Diese Variable wird jedesmal um 1 erhöht ->jede msec
   byte_write(Ampel,Ampelwerte[phase]);

   if(count>=zeit[phase])  // Dieser Programmteil wird ausgeführt: 1msec x arraywert von zeit
   {
     count=0;
     phase=phase+1;
     if( phase==8 )   phase=0;
   }
}
 
 //******* Interrupt-Service-Routine (ISR) **************************
void ext_interrupt_isr(void)    
{                            
  int_status = 1;     // Merker für Interrupt
}