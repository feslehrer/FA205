/*
Beschreibung:     Übung für externen Interrupt
                  Je Raster des Drehgebers werden 2 Counts erkannt.
                  Wertet Kanal A und B des Drehgebers (Sensor) mit
                  ext_interrupt0 und ext_interrupt1 aus. Die Interrupts
                  sind auf negative Flanke getriggert.
                  In interrupt.h muss:
                  #define _FALLING_EDGE_TRIGGER_
                  aktiviert sein.
Erstellt am:       13.12.2016
Letzte Änderung:   23.07.2019

Autor:             Rahm
*/

#include "controller.h"

#define Sensor _PORTD_
#define B1_A     2
#define B1_B     3
#define Taster   _PORTB_
#define Reset    2
#define M        48			  // Impulse/Umdrehung

#define MAXCOUNTS 2000

volatile uint16_t counts = 0;

void setup (void)
{  /* Initialisierungen */
  bit_init(Sensor,B1_B,IN);
  bit_init(Sensor,B1_A,IN);
  bit_init(Reset,Taster,IN);
  
  lcd_init();
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("counts =");
	lcd_setcursor(2,1);
	lcd_print("turns  =");
  
  ext_interrupt_init(ext_interrupt_isr);
  ext_interrupt1_init(ext_interrupt1_isr);
}

// Funktion main()
void main(void)
{
  volatile uint16_t turns, wert;

  setup();
  ext_interrupt_enable();
  ext_interrupt1_enable();
  
  while(1)                          // Endlosschleife loop()
  {
    lcd_setcursor(1,9);
    lcd_int(counts);
    turns = counts / M;
    lcd_setcursor(2,9);		
    lcd_int(turns);

    if (!bit_read(Taster,Reset)) counts = 0;  // Reset, wenn Drehknopf gedrückt!
  }
}

void ext_interrupt_isr(void)
{ // Interrupt bei fallender Flanke an A
  if (bit_read(Sensor,B1_B) == 1)
  {
    if (counts != 0)  counts--;
  }
  else
  {
    if (counts < MAXCOUNTS ) counts++;
  }
}

void ext_interrupt1_isr(void)
{ // Interrupt bei fallender Flanke an B
  if (bit_read(Sensor,B1_A) == 0)
  {
    if (counts != 0)  counts--;
  }
  else
  {
    if (counts < MAXCOUNTS ) counts++;
  }
}