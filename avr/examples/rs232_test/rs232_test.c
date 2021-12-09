/* 
  Beschreibung:	Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für RS232 Sende- und Empfangsroutinen
                Ein Byte wird von der seriellen Schnittstelle gelesen,
                aufs Display geschrieben und als Echo  zurückgeschickt.

  Autor:            Rolf Rahm
  Datum:            19.02.2015
  Letzte Änderung:  10.10.2015
*/
#include "controller.h"

// Funktionen
void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  rs232_init();
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("RS232");
  lcd_setcursor(2,1);
  lcd_print("Char: ");
  rs232_print("Hallo Welt!\n");			// Zeichenkette senden!
}

// Funktion main()
int main(void)
{
  uint8_t c;

  setup();
  
  while(1)                            // Endlosschleife
  {
    while((c=rs232_get()) == '\0');   // Warten bis Zeichen empfangen!
    rs232_put(c);                     // Echo an Terminal senden
    lcd_setcursor(2,7);
    lcd_char(c);                      // Anzeige auf Display
  }
}
