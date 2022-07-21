/*
 Beschreibung:      Inkrementaldrehgeber mit Interrupt auswerten.
   
 Erstellt am:       05.12.2017
 Letzte Änderung:
 
 Autor:             Rahm
*/

#include "controller.h"

#define Sensor   _PORTD_
#define B1_A     2			  // INT0  Definition nicht erforderlich!
#define B1_B     3        // INT1

#define Taster   _PORTB_
#define Reset    2			  // Taster des Drehgebers ist Low-aktiv

#define M        24			  // Impulse/Umdrehung
#define MAXCOUNTS 0xffff  // Maximaler Zählwert

volatile uint16_t counts;	// Variablen, die in ISR verändert werden, immer als volatile

void setup (void)
{  /* Initialisierungen */
  bit_init(Sensor,B1_B,IN);
  bit_init(Taster,Reset,IN);

  lcd_init();
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("counts =");
	lcd_setcursor(2,1);
	lcd_print("turns  =");

  ext_interrupt_init(ext_interrupt_isr);
}

// Funktion main()
void main(void)
{
	uint16_t turns;
	
  setup();
  ext_interrupt_enable();
  
  while(1)                          // Endlosschleife loop()
  {
    lcd_setcursor(1,8);
    lcd_int(counts);
    turns = counts / M;
    lcd_setcursor(2,8);		
    lcd_int(turns);

    if (!bit_read(Taster,Reset)) counts = 0;  // Reset, wenn Drehknopf gedrückt!
  }
}

void ext_interrupt_isr(void)
{
  volatile uint8_t dir;
	
  dir = bit_read(Sensor,B1_B);	// dir = 1: Linksturn
																// dir = 0: Rechtsturn
  if (dir == 1)
  {
    if (counts > 0) 
      counts--;
  }
  else
  {
    if (counts < MAXCOUNTS ) 
			counts++;
  }
}


