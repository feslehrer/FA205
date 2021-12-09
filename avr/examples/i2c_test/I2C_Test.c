/* 
  Beschreibung: Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für I2C-Programmierung
         
  Autor:            Rolf Rahm
  Datum:            21.05.2015
  Letzte Änderung:  09.10.2015
*/

// Einbinden der Bibliotheken der Technischen Richtlinie FA205
#include "controller.h"

// I2C-Testfunktion
// Portexpander PCF8575 am i2c-Bus
//***** verwendete I2C-Adressen und Kontrollbyte für den PCF8574 ****************
#define ADDR_R 	0x41	// 01000001	->	0100 fest, 000 durch Jumper, 1 Wert lesen		
#define ADDR_W  0x40	// 01000000	->	0100 fest, 000 durch Jumper, 0 Wert schreiben		

// lokale Funktionsprototypen
void port_out(uint8_t wert);
uint8_t port_in(void);
void print_hex(uint8_t wert);

// Funktionsdefinitionen
void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  //i2c_init();
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("I2C Test");
}

// Funktion main()
// Auf den Portexpander werden nacheinander zwei Werte (0x55; 0xAA)
// über den I2C-Bus ausgegeben. Zur Kontrolle werden die beiden 
// Werte jeweils wieder eingelesen und Hexadezimal angezeigt.
void main(void)
{
  uint8_t   wert;
  
  setup();
  
  while(1)
  {
    port_out(0x55);            // Portausgabe: 0101 0101
    lcd_setcursor(2,1);
    lcd_print("x55-> ");	
    wert = port_in();          // Port zurücklesen
    print_hex(wert);           // Anzeige am Display

    delay_ms(1000);
	
    port_out(0xaa);            // Portausgabe: 1010 1010
    lcd_setcursor(2,1);
    lcd_print("xAA-> ");
    wert = port_in();          // Port zurücklesen
    print_hex(wert);           // Anzeige am Display

    delay_ms(1000);
  }
}

// 8-Bit Portausgabe (PCF8574) über I2C-Bus
void port_out(uint8_t wert)
{
  uint8_t ack;
   
  i2c_start();                // Startbedingung
  
  ack = i2c_write(ADDR_W);    // Schreibwunsch an Port-Expander senden
  
  delay_100us(2);

  ack = i2c_write(wert);      // port0 ausgeben
  i2c_stop();                 // Stoppbedingung  
}

// 8-Bit Porteingabe (PCF8575) über I2C-Bus
uint8_t port_in(void)
{
  uint8_t wert,ack;
  
  i2c_start();                // Startbedingung
  ack = i2c_write(ADDR_R);    // Lesewunsch an Port-Expander senden
                              // ack (Acknowledge) wird nicht ausgewertet
  wert = i2c_read(NACK);      // Daten vom Port lesen und beenden (NACK)
  i2c_stop();                 // Stoppbedingung
  
  return wert;
}

// Umwandeln eines 8-Bit Dualwertes in 2 Hexstellen und Darstellung an der
// aktuellen Cursorposition
void print_hex(uint8_t wert)
{
  uint8_t temp;
  
  temp = wert>>4;
  if (temp > 9) temp += ('A'-10);
  else temp += '0';
  lcd_char(temp);
  temp = wert & 0x0f; 
  if (temp > 9) temp += ('A'-10);
  else temp += '0';
  lcd_char(temp);
}
