/* 
  Beschreibung:     GS-Motorregelung
                    Implementierung eines digitalen Geschwindigkeitsalgorithmus
  Autor:            Rahm
  Datum:            18.02.2015
  Letzte Änderung:  14.02.2019
*/
#include "controller.h"
#define Taster _PORTB_
#define Start    5

#define Kp   0.2         // Kp = 0.2
#define Tn   1300        // 1300ms
#define Ta   1           // 1ms (Timerinterrupt)

uint8_t status_start = 0;
volatile float x,w,e,y,yp,yi;
volatile float y_alt, e_alt;

void setup (void)   /* Initialisierungen */
{
  lcd_init();
  adc_init();
  pwm_init();
  bit_init(Taster,Start,IN);
  timer1ms_init(timer1ms_isr);
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("Soll:      S4:");
  lcd_setcursor(2,1);
  lcd_print("Ist:       Start");}

// Funktion main()
int main(void)
{
  uint8_t temp;

  setup();

  while(1)                    // Endlosschleife
  {
    bit_toggle(Taster,Start,&status_start);

    lcd_setcursor(1,6);
    if (status_start)
    {
      timer1ms_enable();
      pwm_start();
    }
    else
    {
      timer1ms_disable();
      pwm_stop();
      e_alt = 0;
      y_alt = 0;
    }

    w = adc_in1();
    x = adc_in2();
      
    pwm_duty_cycle(y);      // PWM Tastgrad ändern
    
    lcd_setcursor(1,6);     // Sollwert in % anzeigen
    temp = w*100/255;
    lcd_byte(temp);
    lcd_char('%');

    lcd_setcursor(2,6);     // Istwert in % anzeigen
    temp = x*100/255;
    lcd_byte(temp);
    lcd_char('%');
  }
}

// Geschwindigkeitsalgorithmus (PI-Regler)
void timer1ms_isr(void)
{
  e  = w - x;
  yp = Kp * e;
    
  yi = (Kp * e_alt - Kp * e_alt * Ta / Tn);     // Ta = 1ms; Tn = 1300ms
  
  y = (y_alt + yp - yi);
  
  y_alt = y;
  e_alt = e;
  
  if (y>255) y=255;     // Begrenzung der Stellgröße
  if (y<0)   y = 0;
}