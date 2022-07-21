/*
//	Programm:		Robotersteuerung
//  Beschreibung:	Einfacher Ausweich-Roboter mit Infrarotsensoren
//
//	Datum:			19.12.2019
//	Autor:			Rahm
*/
#include "controller.h"

#define SensorD _PORTD_
#define IRL    0        // PD0
#define IRM    1        // PD1
#define IRR    7        // PD7

const uint8_t directions[9][4] = {
								{0,0,0,0},     // stopp
								{0,1,0,1},     // vor
								{1,0,1,0},     // rueck
								{0,0,0,1},     // linksvor
								{0,1,0,0},     // rechtsvor
								{1,0,0,1},     // linksdreh
								{0,1,1,0},     // rechtsdreh
								{1,0,0,0},     // linksrueck
								{0,0,1,0}      // rechtsrueck
};

enum Richtungen {STOPP,VORWAERTS,RUECKWAERTS,LINKSVOR,RECHTSVOR,LINKSDREH,RECHTSDREH,LINKSRUECK,RECHTSRUECK};

//Funktionsprototypen
void robby_richtung(uint8_t dir, uint8_t speed);

void setup (void)   // Initialisierungen
{ 
  // IR-Sensorsignale
  bit_init(SensorD,IRL,IN);
  bit_init(SensorD,IRM,IN);
  bit_init(SensorD,IRR,IN);
    
  // Motorsignale 
  bit_init(_PORTD_,2,OUT);      // Input 1
  bit_init(_PORTB_,2,OUT);      // Input 2 
  bit_init(_PORTB_,4,OUT);      // Input 3
  bit_init(_PORTB_,5,OUT);      // Input 4
  
  pwm_init();                   // Enable A (pwm)
  pwm2_init();                  // Enable B (pwm)
  
  adc_init();
}

int main (void)
{
  uint8_t speed;
  
  setup();
  
  pwm_start();
  pwm2_start();
  
  while(1)
  {
    speed = adc_in1();
     
    if      (!bit_read(SensorD,IRM))  
      robby_richtung(RECHTSRUECK, speed);
    else if (!bit_read(SensorD,IRL))  
      robby_richtung(RECHTSVOR, speed);
    else if (!bit_read(SensorD,IRR))
      robby_richtung(LINKSVOR, speed);
    else if (!bit_read(SensorD,IRM) && !bit_read(SensorD,IRL))
      robby_richtung(RECHTSDREH, speed);
    else if (!bit_read(SensorD,IRM) && !bit_read(SensorD,IRR))
      robby_richtung(LINKSDREH, speed);    
    else if (!bit_read(SensorD,IRR) && !bit_read(SensorD,IRL))
      robby_richtung(RECHTSRUECK, speed);
    else if (!bit_read(SensorD,IRM) && !bit_read(SensorD,IRL) && !bit_read(SensorD,IRR)) 
      robby_richtung(RECHTSRUECK,speed);
    else 
      robby_richtung(VORWAERTS, speed);
  }
}

// Bewegungsrichtung des Roboters festlegen
// dir:   0 ... 9         (Richtungen: STOPP,VORWAERTS,RUECKWAERTS,
//                                     LINKSVOR,RECHTSVOR,LINKSDREH,
//                                     RECHTSDREH,LINKSRUECK,RECHTSRUECK)
// speed: 0 ... 255       (Geschwindigkeit)
void robby_richtung(uint8_t dir, uint8_t speed)
{ 
  //Bewegungsrichtung
  bit_write(_PORTD_,2,directions[dir][0]);       // Input 1
  bit_write(_PORTB_,2,directions[dir][1]);       // Input 2
  bit_write(_PORTB_,4,directions[dir][2]);       // Input 3
  bit_write(_PORTB_,5,directions[dir][3]);       // Input 4
  
  pwm_duty_cycle(speed);
  pwm2_duty_cycle(speed);
}
