/*
//	Programm:		Robotersteuerung mit Linienverfolger
//  Beschreibung:
//
//	Datum:			2019.02.01
//	Autor:			Rahm
*/
#include "controller.h"

#define SensorD _PORTD_
#define IRL    0        // PD0
#define IRM    1        // PD1
#define IRR    7        // PD7
#define RflxL  4        // PD4
#define RflxM  5        // PD5
#define RflxR  6		    // PD6
#define SensorB _PORTB_
#define ENDL   1        // PB1
#define ENDR   0        // PB0

const int directions[9][4] = {
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

enum Richtungen {STOPP,VORWAERTS,RUECKWAERTS,LINKSVOR,RECHTSVOR,
                  LINKSDREH,RECHTSDREH,LINKSRUECK,RECHTSRUECK};

//Funktionsprototypen
void robby_richtung( uint8_t dir, uint8_t speed, int16_t fade );

void setup (void)   // Initialisierungen
{
  //lcd_init();
  //lcd_clear();
  
  pwm_init();       // Motor rechts Speed
  pwm2_init();      // Motor links  Speed
  
    // Motorsignale 
  bit_init(_PORTD_,2,OUT);      // Input 1
  bit_init(_PORTB_,2,OUT);      // Input 2 
  bit_init(_PORTB_,4,OUT);      // Input 3
  bit_init(_PORTB_,5,OUT);      // Input 4
  
  bit_init(SensorD,RflxL,IN);
  bit_init(SensorD,RflxM,IN);
  bit_init(SensorD,RflxR,IN);

  pwm2_start();
  pwm_start();
  
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
  
  setup();
  
  pwm_start();
  pwm2_start();
  
  while(1)
  {
    speed = 200;//adc_in1();
    
    sensor = ~byte_read(SensorD); // Sensoren sind 0-aktiv
    #ifdef LCD_I2C
      // Anzeigen der Sensorwerte
      //lcd_setcursor(1,1);
      //lcd_print("PORTB: ");
      //lcd_debug((~byte_read(_PORTB_)) & ((1<<EndL)|(1<<EndR))); //EndL & EndR anzeigen
      lcd_setcursor(2,1);
      lcd_print("PORTD: ");
      lcd_debug(sensor & ((1<<IRL)|(1<<IRM)|(1<<IRR)|(1<<RflxL)|(1<<RflxM)|(1<<RflxR)));
    #endif
    
    switch(sensor & ((1<<RflxL) | (1<<RflxM) | (1<<RflxR)))
    {
      case (1<<RflxL):                     robby_richtung(VORWAERTS, speed-20,  120); break;
      case (1<<RflxR):                     robby_richtung(VORWAERTS, speed-20, -120); break;
      case (1<<RflxM):                     robby_richtung(VORWAERTS, speed   ,    0); break;
      case ((1<<RflxM)|(1<<RflxL)):        robby_richtung(VORWAERTS, speed-10,   90); break;
      case ((1<<RflxM)|(1<<RflxR)):        robby_richtung(VORWAERTS, speed-10,  -90); break;
      default:                             robby_richtung(RECHTSDREH,   120  ,    0); break;
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