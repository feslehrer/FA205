/*
//
//	Programm:		Vorlage f�r C-Programm FA205
//  Beschreibung:
//
//	Datum:			2016.10.09
//	Autor:		
//
*/
#include "controller.h"

#define IRL _PORTD_,0        // PD0
#define IRM _PORTD_,1        // PD1
#define IRR _PORTD_,7        // PD7

#define INPUT_1 _PORTD_,2
#define INPUT_2 _PORTB_,2
#define INPUT_3 _PORTB_,4
#define INPUT_4 _PORTB_,5

//Funktionsprototypen
void setup (void)   // Initialisierungen
{ 
  // IR-Sensorsignale
  bit_init(IRL,IN);
  bit_init(IRM,IN);
  bit_init(IRR,IN);
  
  // Motorsignale 
  bit_init(INPUT_1,OUT);      // Input 1
  bit_init(INPUT_2,OUT);      // Input 2 
  bit_init(INPUT_3,OUT);      // Input 3
  bit_init(INPUT_4,OUT);      // Input 4

  pwm_init();                   // Enable A (pwm)
  pwm2_init();                  // Enable B (pwm)
  
}

// Bewegungsrichtungen festlegen
void stopp(void)
{
  bit_write(INPUT_1,0);       // Input 1 = 0
  bit_write(INPUT_2,0);       // Input 2 = 0
  bit_write(INPUT_3,0);       // Input 3 = 0
  bit_write(INPUT_4,0);       // Input 4 = 0
}

void vorwaerts(void)
{
  bit_write(INPUT_1,0);       // Input 1 = 1
  bit_write(INPUT_2,1);       // Input 2 = 0
  bit_write(INPUT_3,0);       // Input 3 = 0
  bit_write(INPUT_4,1);       // Input 4 = 1
}

void rueckwaerts(void)
{
  bit_write(INPUT_1,1);       // Input 1 = 0
  bit_write(INPUT_2,0);       // Input 2 = 1
  bit_write(INPUT_3,1);       // Input 3 = 1
  bit_write(INPUT_4,0);       // Input 4 = 0
}

void drehlinks(void)
{
  bit_write(INPUT_1,1);       // Input 1 = 0
  bit_write(INPUT_2,0);       // Input 2 = 1
  bit_write(INPUT_3,0);       // Input 3 = 0
  bit_write(INPUT_4,1);       // Input 4 = 1
}

void drehrechts(void)
{
  bit_write(INPUT_1,0);       // Input 1 = 1
  bit_write(INPUT_2,1);       // Input 2 = 0
  bit_write(INPUT_3,1);       // Input 3 = 1
  bit_write(INPUT_4,0);       // Input 4 = 0
}


int main (void)
{
  
	setup();
  
  // PWM über Enable A und B
  pwm_start();
  pwm2_start();
  
  pwm_duty_cycle(100);
  pwm2_duty_cycle(100);
  
	while(1)
	{
     if      (bit_read(IRM)==0)   stopp();
     else if (bit_read(IRL)==0)   drehrechts();
     else if (bit_read(IRR)==0)   drehlinks();
     else                         vorwaerts();
	}
}
