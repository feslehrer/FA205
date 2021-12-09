/* 
  Beschreibung: Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für ms-Timer 
                Der Timer löst alle 1ms einen Interrupt aus und startet die Funktion timer1ms_isr()

  Autor:            Rolf Rahm
  Datum:            18.02.2015
  Letzte Änderung:  02.02.2016
*/
#include "controller.h"

void print_time(uint8_t time);

uint16_t ms = 0;
uint8_t  ss = 0;
uint8_t  mm = 0;
uint8_t  hh = 0;

// Funktionen
void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  timer1ms_init( timer1ms_isr );

  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("Timer");

  timer1ms_enable();
}

// Timer-ISR
void timer1ms_isr(void)
{
  ms++;	
  if (ms > 999)
  {
    ss++;
    ms = 0;
    if (ss > 59)
    {
      mm++;
      ss = 0;
      if (mm > 59)
      {
        hh++;
        mm = 0;
        if (hh > 23)
          hh = 0;						
      }
    }
  }
}

// Funktion main()
int main(void)
{	
  setup();
	
  while(1)                 // Endlosschleife
  {
    lcd_setcursor(2,1);
    print_time(hh);
    lcd_char(':');
    print_time(mm);
    lcd_char(':');
    print_time(ss);
  }
}

void print_time(uint8_t time)
{ // Ausgabe einer Dezimalzahl 00..59
  // auf das Display
  uint8_t buf;
  
  buf = time / 10;
  lcd_char(buf+'0');      // 10er-Stelle anzeigen
  buf = time % 10;
  lcd_char(buf+'0');      // 1er-Stelle anzeigen
}

