/*
//
//	Programm:		Vorlage für C-Programm FA205
//  Beschreibung:
//
//	Datum:			2016.10.09
//	Autor:		
//
*/
#include "controller.h"

#define Sensor _PORTD_
#define IRL    0        // PD0
#define IRM    1        // PD1
#define IRR    7        // PD7


//Funktionsprototypen
void setup (void)   // Initialisierungen
{ 
  // Motorsignale 
  bit_init(_PORTD_,2,OUT);      // Input 1
  bit_init(_PORTB_,2,OUT);      // Input 2 
  bit_init(_PORTB_,4,OUT);      // Input 3
  bit_init(_PORTB_,5,OUT);      // Input 4
  
  pwm_init();                   // Enable A (pwm)
  pwm2_init();                  // Enable B (pwm)
  
  // IR-Sensorsignale
  bit_init(Sensor,IRL,IN);
  bit_init(Sensor,IRM,IN);
  bit_init(Sensor,IRR,IN); 
}

// Bewegungsrichtungen festlegen
void stopp(void)
{
  bit_write(_PORTD_,2,0);       // Input 1 = 0
  bit_write(_PORTB_,2,0);       // Input 2 = 0
  bit_write(_PORTB_,4,0);       // Input 3 = 0
  bit_write(_PORTB_,5,0);       // Input 4 = 0
}

void vorwaerts(void)
{
  bit_write(_PORTD_,2,0);       // Input 1 = 1
  bit_write(_PORTB_,2,1);       // Input 2 = 0
  bit_write(_PORTB_,4,0);       // Input 3 = 0
  bit_write(_PORTB_,5,1);       // Input 4 = 1
}

void rueckwaerts(void)
{
  bit_write(_PORTD_,2,1);       // Input 1 = 0
  bit_write(_PORTB_,2,0);       // Input 2 = 1
  bit_write(_PORTB_,4,1);       // Input 3 = 1
  bit_write(_PORTB_,5,0);       // Input 4 = 0
}

void drehlinks(void)
{
  bit_write(_PORTD_,2,1);       // Input 1 = 0
  bit_write(_PORTB_,2,0);       // Input 2 = 1
  bit_write(_PORTB_,4,0);       // Input 3 = 0
  bit_write(_PORTB_,5,1);       // Input 4 = 1
}

void drehrechts(void)
{
  bit_write(_PORTD_,2,0);       // Input 1 = 1
  bit_write(_PORTB_,2,1);       // Input 2 = 0
  bit_write(_PORTB_,4,1);       // Input 3 = 1
  bit_write(_PORTB_,5,0);       // Input 4 = 0
}


int main (void)
{
  
	setup();
  
  //bit_write(_PORTB_,3,1);      // Enable A
  //bit_write(_PORTD_,3,1);      // Enable B
  pwm_start();
  pwm2_start();
  
  pwm_duty_cycle(100);
  pwm2_duty_cycle(100);
  
	while(1)
	{
     if (bit_read(Sensor,IRM)==0)  stopp();
     else if (bit_read(Sensor,IRL)==0)  drehrechts();
     else if (bit_read(Sensor,IRR)==0)  drehlinks();
     else vorwaerts();
	}
}
