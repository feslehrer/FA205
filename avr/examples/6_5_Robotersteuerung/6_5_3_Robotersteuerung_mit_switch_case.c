/*
//	Programm:		      Robotersteuerung
//  Beschreibung:	    Einfacher Ausweich-Roboter mit Front-IR-Sensoren
//
//	Datum:			      19.12.2019
//  letzte Änderung:  23.12.2019
//	Autor:			      Rahm
*/
#include "controller.h"

#define SensorD _PORTD_
#define IRL     0        // IR-Sensoren
#define IRM     1        
#define IRR     7         
#define RflxL   4        // Linienabtastung mit
#define RflxM   5        // Reflexlichttaster
#define RflxR   6
#define SensorB _PORTB_
#define EndL    1        // Endschalter vorne
#define EndR    0

// 2 dimensionales Array für Einstellung der Roboterrichtung
// Reihenfolge der Steuersignale: {Input1, Input2, Input3, Input4}
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

// Namen für Richtungen festlegen
enum Richtungen {STOPP,        // 0
                 VORWAERTS,    // 1
                 RUECKWAERTS,  // 2
                 LINKSVOR,     // 3
                 RECHTSVOR,    // 4
                 LINKSDREH,    // 5
                 RECHTSDREH,   // 6
                 LINKSRUECK,   // 7
                 RECHTSRUECK   // 8
                 };

//Funktionsprototypen
void robby_richtung(uint8_t dir, uint8_t speed, int16_t fade);

void setup (void)   // Initialisierungen
{ 
  // IR-Sensorsignale
  //bit_init(SensorD,IRL,IN);
  //bit_init(SensorD,IRM,IN);
  //bit_init(SensorD,IRR,IN);
  byte_init(SensorD,IN);
  //byte_init(SensorB,IN);
  // Motorsignale 
  bit_init(_PORTD_,2,OUT);      // Input 1
  bit_init(_PORTB_,2,OUT);      // Input 2 
  bit_init(_PORTB_,4,OUT);      // Input 3
  bit_init(_PORTB_,5,OUT);      // Input 4
  
  pwm_init();                   // Enable A (pwm)
  pwm2_init();                  // Enable B (pwm)
  
  adc_init();
  #ifdef LCD_I2C
    lcd_init();
    lcd_clear();
    lcd_setcursor(1,1);
    lcd_print("Robby01");
  #endif
}

int main (void)
{
  volatile uint8_t speed, sensor;
  volatile int8_t  fade;
  
  setup();
  
  pwm_start();
  pwm2_start();
  
  while(1)
  {
    //speed = adc_in1();
    fade  = 127 - adc_in1();
    speed = 140;
    
    sensor = ~byte_read(_PORTD_); // Sensoren sind 0-aktiv

    #ifdef LCD_I2C
      // Anzeigen der Sensorwerte
      //lcd_setcursor(1,1);
      //lcd_print("PORTB: ");
      //lcd_debug((~byte_read(_PORTB_)) & ((1<<EndL)|(1<<EndR))); //EndL & EndR anzeigen
      lcd_setcursor(2,1);
      lcd_print("PORTD: ");
      lcd_debug(sensor & ((1<<IRL)|(1<<IRM)|(1<<IRR)|(1<<RflxL)|(1<<RflxM)|(1<<RflxR)));
    #endif
    
    switch(sensor & ((1<<IRL) | (1<<IRM) | (1<<IRR)))
    {
      case (1<<IRM):                     robby_richtung(RECHTSRUECK, speed, fade); break;
      case (1<<IRL):                     robby_richtung(RECHTSVOR,   speed, fade); break;
      case (1<<IRR):                     robby_richtung(LINKSVOR,    speed, fade); break;
      case ((1<<IRM)|(1<<IRL)):          robby_richtung(RECHTSDREH,  speed, fade); break;
      case ((1<<IRM)|(1<<IRR)):          robby_richtung(LINKSDREH,   speed, fade); break;
      case ((1<<IRR)|(1<<IRL)):          
      case ((1<<IRM)|(1<<IRR)|(1<<IRL)): robby_richtung(RUECKWAERTS, speed, fade); delay_ms(1000); 
                                         robby_richtung(RECHTSDREH,speed,fade);    delay_ms(1000);
                                         break;
                                         
      default:                           robby_richtung(VORWAERTS,   speed, fade); break;
    }
  }
}

// Bewegungsrichtung des Roboters festlegen
// dir:   0 ... 9         (Richtungen: STOPP,VORWAERTS,RUECKWAERTS,
//                                     LINKSVOR,RECHTSVOR,LINKSDREH,
//                                     RECHTSDREH,LINKSRUECK,RECHTSRUECK)
// speed: 0 ... 255       (Geschwindigkeit)
// fade:  -255 ... +255   (unsymmetrische Motoransteuerung: fade negativ = Motor links  +
//                                                                         Motor rechts -)
void robby_richtung(uint8_t dir, uint8_t speed, int16_t fade)
{
  int16_t left, right;
  
  //Bewegungsrichtung
  bit_write(_PORTD_,2,directions[dir][0]);       // Input 1
  bit_write(_PORTB_,2,directions[dir][1]);       // Input 2
  bit_write(_PORTB_,4,directions[dir][2]);       // Input 3
  bit_write(_PORTB_,5,directions[dir][3]);       // Input 4
  
  //Differenzial
  left  = (int16_t)speed - fade;
  right = (int16_t)speed + fade;
  
  //Bereichsbegrenzung für PWM
  if (left > 255)     left = 255;
  else if (left < 0)  left = 0;
  if (right > 255)    right = 255;
  else if (right < 0) right = 0;
  
  pwm_duty_cycle(left);
  pwm2_duty_cycle(right);
}