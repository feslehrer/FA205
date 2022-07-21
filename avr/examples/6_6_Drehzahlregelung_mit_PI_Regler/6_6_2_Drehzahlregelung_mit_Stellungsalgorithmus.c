/* 
  Beschreibung:     GS-Motorregelung
                    Implementierung eines einfachen digitalen Stellungsalgorithmus
  Autor:            Rahm
  Datum:            18.02.2015
  Letzte Änderung:  13.02.2019
*/
#include "controller.h"
#define Taster _PORTB_
#define Start    5

#define Kp   20           // Kp = 20 / 100 = 0.2
#define Tn   1300         // Tn = 1300ms
#define Ta   1            // Ta = 1ms (Timer1ms)

uint8_t status_start = 0;
volatile int32_t x,w,e,y,yp,yi,e_k;

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
  lcd_print("Ist:       Start");
}

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
      e_k = 0;
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

void timer1ms_isr(void)
{
  e = w - x;
  yp = Kp * e;
    
  e_k += e;
  yi = Kp * e_k * Ta / Tn;  // Ta = 1ms; Tn = 1300ms
  
  y = yp + yi;
  y = y / 100;              // :100, wegen Kp * 100 
  
  if (y>255) y=255;
  if (y<0)   y = 0;
}