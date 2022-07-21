/*
 Beschreibung:      Übung für externen Interrupt
   
 Erstellt am:       13.12.2016
 Letzte Änderung:
 
 Autor:             Rahm
*/

#include "controller.h"

#define Sensor _PORTD_
#define B1_A     2
#define B1_B     3
#define Taster   _PORTB_
#define Reset   2
#define m       20 // Impulse/cm
#define MAXCOUNTS 2000

volatile uint16_t counts = 0;

void setup (void)
{  /* Initialisierungen */
  bit_init(Sensor,B1_B,IN);
  bit_init(Reset,Taster,IN);
  
  lcd_init();
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("counts =");
  lcd_setcursor(2,1);
  lcd_print("s =");
  ext_interrupt_init(ext_interrupt_isr);
}

// Funktion main()
void main(void)
{
  volatile uint16_t temp, wert;

  setup();
  ext_interrupt_enable();
  
  while(1)                          // Endlosschleife loop()
  {
    lcd_setcursor(1,9);
    lcd_int(counts);
    lcd_setcursor(2,4);
    wert = counts * 100 / m;
    temp = wert / 100;
    lcd_int(temp);
    lcd_char(',');

    wert = wert % 100;
    temp = wert / 10;    
    lcd_char(temp + '0');
    temp = wert % 10;
    lcd_char(temp + '0');
    
    lcd_print(" cm");

    if (!bit_read(Reset,Taster)) counts = 0;
  }
}

void ext_interrupt_isr(void)
{
  if (bit_read(Sensor,B1_B))       // Wenn B1_A noch 0 ist...
  {
    if (counts != 0)  counts--;
	}
  else
  {
      if (counts < MAXCOUNTS ) counts++;
  }
}
