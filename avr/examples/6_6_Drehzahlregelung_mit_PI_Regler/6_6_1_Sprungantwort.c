/* 
  Beschreibung:     GS-Motorregelung
                    Aufnahme der Sprungantwort des Antriebs
  Autor:            Rahm
  Datum:            18.02.2015
  Letzte Änderung:  14.02.2019
*/
#include "controller.h"
#define Taster _PORTB_
#define Start    5

uint8_t status_start = 0;
volatile uint8_t x,w,y;

void setup (void)   /* Initialisierungen */
{
  lcd_init();
  adc_init();
  pwm_init();
  bit_init(Taster,Start,IN);
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("Soll:");
  lcd_setcursor(2,1);
  lcd_print("Ist:");
}

// Funktion main()
int main(void)
{
  uint8_t temp;

  setup();

  while(1)                    // Endlosschleife
  {
    bit_toggle(Taster,Start,&status_start);
    
    w = adc_in1();
    x = adc_in2();
    
    if (status_start)
    {
      y = w;
      pwm_start();
      pwm_duty_cycle(y);      // PWM Tastgrad ändern
    }
    else
    {
      pwm_stop();
    }
    
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
