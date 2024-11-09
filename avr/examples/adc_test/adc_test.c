/*
  Beschreibung:    Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                   Das Programm liest einen Analogwert ein und zeigt ihen am LCD an.

  Bibliotheken:    in_out.c
                   delay.c
                   lcd.c

  Autor:           Rolf Rahm
  Datum:           18.02.2015
  Letzte Änderung: 06.02.2016
*/

#include "controller.h"

// Funktionen
void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  adc_init();

  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("ADC Test");
}

// Funktion main()
int main(void) 
{
  uint8_t buf;
  uint8_t blink = 0;

  setup();

  while(1)                // Endlosschleife loop()
  {	
    buf = adc_in1();
    lcd_setcursor(2,2);
    lcd_byte(buf);
    if(blink) 
    {
      lcd_print("   *");
      blink = 0;
    }
    else
    {
      lcd_print("    ");
      blink = 1;
    }    

    delay_ms(500);
  }
}
