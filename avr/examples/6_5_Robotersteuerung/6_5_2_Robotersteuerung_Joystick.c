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
#include "nunchuk.h"

#define Input1 _PORTD_
#define Input2 _PORTB_
#define Input3 _PORTB_
#define Input4 _PORTB_

#define dir1   2        // Port D.2
#define dir2   2        // Port B.2
#define dir3   4        // Port B.4
#define dir4   5        // Port B.5

#define tol    5

//Funktionsprototypen
void RoboterMotoren( int8_t speedL, int8_t speedR);
int8_t normalize( uint8_t _joystick);

void setup (void)   // Initialisierungen
{
  i2c_init();
  nunchuk_init();
  lcd_init();
  lcd_clear();
  
  pwm_init();       // Motor rechts Speed
  pwm2_init();      // Motor links  Speed
  
  bit_init(Input1,dir1,OUT);
  bit_init(Input2,dir2,OUT);
  bit_init(Input3,dir3,OUT);
  bit_init(Input4,dir4,OUT);
  
  RoboterMotoren(127,127);
  pwm2_start();
  pwm_start();
}

int main (void)
{
  int8_t M_links,M_rechts;
  volatile int8_t X_norm, Y_norm;
  
	setup();
	
	while(1)
	{
		nunchuk_read();  // Einlesen der Joystick-Variablen
    nunchuk_debug(); // Anzeige auf dem Display
    
    // Normalisieren der Joystick-Werte
    // _joy:    0....128...255
    // _norm:-128.....0....127
    X_norm = normalize(X_joy);
    Y_norm = normalize(Y_joy);
    
    if ( Y_norm < -tol && X_norm < -tol )      // Rechtsrück (oder Rechtsdrehen)
     { M_links = -X_norm; M_rechts =  Y_norm;}   
    else if ( Y_norm < -tol && X_norm > tol ) // Linksrück (oder Linksdrehen)
     { M_links = -X_norm; M_rechts = -Y_norm;}
    else if ( X_norm < -tol )                  // Linksdrehen    
     { M_links =  X_norm; M_rechts = -X_norm; }
    else if ( X_norm > tol )                   // Rechtsdrehen
     { M_links =  X_norm; M_rechts = -X_norm; }
    else                                       // Vorwärts/Rückwärts/Stopp
     { M_links = Y_norm; M_rechts = Y_norm; }

    RoboterMotoren(M_links,M_rechts); 
	}
}

void RoboterMotoren( int8_t speedL, int8_t speedR)
{ 
  //***** linke Raupe
  if (speedL > tol)       
  {
    bit_write(Input2,dir2,1);   // vor
    bit_write(Input1,dir1,0);
    pwm2_duty_cycle(speedL*2+1);
  }
  else if (speedL < -tol)
  {
    bit_write(Input1,dir1,1);   // rück
    bit_write(Input2,dir2,0);
    pwm2_duty_cycle((-speedL)*2-1);
  }
  else
  {
    bit_write(Input1,dir1,0);   // Stopp
    bit_write(Input2,dir2,0);
    pwm2_duty_cycle(0);
  }
  
  //***** rechte Raupe 
  if (speedR > tol)
  {  
    bit_write(Input4,dir4,1);   // vor
    bit_write(Input3,dir3,0);
    pwm_duty_cycle(speedR*2+1);
  }
  else if (speedR < -tol)
  {
    bit_write(Input3,dir3,1);   // rück
    bit_write(Input4,dir4,0);
    pwm_duty_cycle((-speedR)*2-1);
  }
  else
  {
    bit_write(Input3,dir3,0);   // Stopp
    bit_write(Input4,dir4,0);
    pwm_duty_cycle(0);
  }
  return;
}  

int8_t normalize( uint8_t _joystick)
{
  int8_t _norm;
  if (_joystick > 128)    _norm = _joystick-128;
  else                    _norm = 128+_joystick;
  
  return _norm;
}