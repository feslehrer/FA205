/*
Beschreibung:      Übung für externen Interrupt
                   Je Raster des Drehgebers werden 4 Counts erkannt.
                   Wertet Kanal A und B des Drehgebers (Sensor) mit 
                   ext_interrupt0 und ext_interrupt1 aus. Die Interrupts
                   sind auf positive und negative Flanke getriggert.
                   In interrupt.h muss:
                       #define _ANY_EDGE_TRIGGER_
                   aktiviert sein.
Erstellt am:       13.12.2016
Letzte Änderung:

Autor:             Rahm
*/

#include "controller.h"

#define _PORTD_,B1_A     2
#define _PORTD_,B1_B     3
#define _PORTB_,Reset    2
#define M        96			  // Impulse/Umdrehung

#define MAXCOUNTS 2000

volatile uint16_t counts = 0;

void setup (void)
{  /* Initialisierungen */
  bit_init(B1_B,IN);
  bit_init(B1_A,IN);
  bit_init(Reset,IN);
  
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

    if (!bit_read(Reset)) counts = 0;  // Reset, wenn Drehknopf gedrückt!
  }
}

void ext_interrupt_isr(void)
{   
  if (bit_read(B1_A) == 0)
  {  // Interrupt bei fallender Flanke an A
    if (bit_read(B1_B) == 1)
    {
      if (counts != 0)  counts--;
    }
    else
    {
      if (counts < MAXCOUNTS ) counts++;
    }
  }  
  else
  {  //Interrupt bei steigender Flanke an A
    if (bit_read(B1_B) == 0)
    {
      if (counts != 0)  counts--;
    }
    else
    {
      if (counts < MAXCOUNTS ) counts++;
    }
  }
}

void ext_interrupt1_isr(void)
{       
  if (bit_read(B1_B) == 0) 
  {  // Interrupt bei fallender Flanke an B
    if (bit_read(B1_A) == 0)
    {
      if (counts != 0)  counts--;
    }
    else  
    {
      if (counts < MAXCOUNTS ) counts++;
    }
  }
  else   
  {  // Interrupt bei steigender Flanke an B
    if (bit_read(B1_A) == 1)
    {
      if (counts != 0)  counts--;
    }
    else
    {
      if (counts < MAXCOUNTS ) counts++;
    }
  }
}