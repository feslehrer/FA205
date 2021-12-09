/* 
  Beschreibung: Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für Pulsweitenmodulation (PWM)
                Am PWM-Ausgang (PB.3) wird eine PWM erzeugt. Der Tastgrad wird mit dem
                Analogwert an CH1 eingestellt.

  Autor:            Rolf Rahm
  Datum:            18.02.2015
  Letzte Änderung:  02.02.2016
*/
#include "controller.h"

// Funktionen
void setup (void)   /* Initialisierungen */
{
  lcd_init();
  adc_init();
  pwm_init();
  bit_init(_PORTC_,0,IN);           // Taster !
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("PWM Test");
}

// Funktion main()
int main(void)
{
  uint16_t temp;

  setup();

  while(1)                        // Endlosschleife
  {
    if (bit_read(_PORTC_,0) == 1)
    {
      pwm_start();
      lcd_setcursor(2,1);
      lcd_print(" on ");
    }
    else
    {
      pwm_stop();
      lcd_setcursor(2,1);
      lcd_print(" off");
    }
    temp = adc_in1();
    pwm_duty_cycle((uint8_t)temp);      // PWM Tastgrad ändern
    lcd_setcursor(2,5);
    temp = 100 - temp*100/255;
    lcd_byte((uint8_t)temp);
    lcd_char('%');
    delay_ms(500);
  }
}
