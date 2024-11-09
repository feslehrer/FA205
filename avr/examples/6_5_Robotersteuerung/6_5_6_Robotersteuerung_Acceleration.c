/*
//
//	Programm:		     Robotersteuerung mit Nunchuk-Joystick
//  Beschreibung:
//
//	Datum:			     2016.10.09
//  letzte Änderung: 27.12.19
//	Autor:
//
*/
#include "controller.h"
#include "nunchuk.h"

#define INPUT_1 _PORTD_,2
#define INPUT_2 _PORTB_,2
#define INPUT_3 _PORTB_,4
#define INPUT_4 _PORTB_,5

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

enum Richtungen {STOPP,VORWAERTS,RUECKWAERTS,LINKSVOR,RECHTSVOR,LINKSDREH,RECHTSDREH,LINKSRUECK,RECHTSRUECK};

//Funktionsprototypen
void robby_richtung( uint8_t dir, uint8_t speed, int16_t fade );

void setup (void)   // Initialisierungen
{
  i2c_init();
  nunchuk_init();
  lcd_init();
  lcd_clear();
  
  pwm_init();       // Motor rechts Speed
  pwm2_init();      // Motor links  Speed
  
  // Motorsignale
  bit_init(INPUT_1,OUT);      // Input 1
  bit_init(INPUT_2,OUT);      // Input 2 
  bit_init(INPUT_3,OUT);      // Input 3
  bit_init(INPUT_4,OUT);      // Input 4
  pwm2_start();
  pwm_start();
}

int main (void)
{
  volatile int8_t X_norm, Y_norm;
  
  setup();
  
  while(1)
  {
    nunchuk_read();  // Einlesen der Acceleration-Werte (X_accel, Y_accel, ...)
    nunchuk_debug(); // Anzeige auf dem Display
    		
    // Normalisieren der Beschleunigungs-Werte
    // _accel:    0....512...1024
    // _norm:  -128.....0....127
    X_norm = X_accel/4-128;
    Y_norm = Y_accel/4-128;
    		
    if      (Y_norm > 10)	  robby_richtung(VORWAERTS,    Y_norm*3, -X_norm);
    else if (Y_norm < -10)  robby_richtung(RUECKWAERTS, (-Y_norm)*3, -X_norm);
    else                    robby_richtung(STOPP,0,0);
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
  bit_write(INPUT_1,directions[dir][0]);       // Input 1
  bit_write(INPUT_2,directions[dir][1]);       // Input 2
  bit_write(INPUT_3,directions[dir][2]);       // Input 3
  bit_write(INPUT_4,directions[dir][3]);       // Input 4
  
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