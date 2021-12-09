/* 
  Beschreibung: Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für I2C-Programmierung
         
  Autor:            Rolf Rahm
  Datum:            21.05.2015
  Letzte Änderung:  04.02.2016
*/

// Einbinden der Bibliotheken der Technischen Richtlinie FA205
#include "controller.h"

// I2C-Testfunktion
//***** verwendete I2C-Adressen und Kontrollbyte für den LM75 ****************
#define ADDR_R 	0x91	// 10010001	->	1001 fest, 000 durch Jumper, 1 Wert lesen		
#define ADDR_W  0x90	// 10010000	->	1001 fest, 000 durch Jumper, 0 Wert schreiben		

// lokale Funktionsprototypen
uint16_t lm75_read(void);

// Funktionsdefinitionen
void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  i2c_init();
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("**LM75**");
}

// Funktion main()
int16_t main(void)
{
  volatile uint8_t   wert;
  
  setup();
  
  while(1)
  {
    wert = lm75_read();
    lcd_setcursor(2,2);
	
	if (wert > 0x7f)		// Temperatur negativ ?
	{
		wert = ~wert;
		wert++;				// 2er-Komplement
		lcd_char('-');		// Minuszeichen
	}		
    lcd_byte(wert);
    lcd_print("°C");

    delay_ms(100);
  }
}

// Einlesen des Temperaturwerts 
uint16_t lm75_read(void)
{
  uint16_t msb,lsb;
  
  i2c_start();                // Startbedingung
  i2c_write(ADDR_W);          // Schreibwunsch an LM75 senden
  i2c_write(0x00);            // Pointerbyte = 0
  i2c_stop();
  
  i2c_start();
  i2c_write(ADDR_R);          // Lesewunsch an LM75 senden
  msb = i2c_read(ACK);        // MSB lesen und weiteren Wert anfordern.
  lsb = i2c_read(NACK);       // LSB lesen
  i2c_stop();                 // Stoppbedingung
  
  return msb;                 // Nur MSB (Genauigkeit: +/- 1°C)
}

