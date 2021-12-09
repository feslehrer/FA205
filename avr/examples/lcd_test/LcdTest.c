/* 
  Beschreibung:     Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                    Beispiel für LCD
								
  Autor:            Rolf Rahm
  Datum:            17.02.2015
  Letzte Änderung:  8.10.2015
*/
#include "controller.h"


void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("LCD Test");
}

// Funktion main()
int main(void)
{
  uint8_t a = 0;

  setup();

  while(1)
  {
    lcd_setcursor(2,1);
    lcd_byte(a);
    lcd_print(" -> ");   
    lcd_char(a);
    delay_ms(1000);
	a++;
  }
}

