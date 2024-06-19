// Technische Richtlinie FA205
// Bibliothek:       lcd.h
// Controller:       atmel ATmega8/ATmega328
// Version:          1.0
// erstellt am:      30.9.2015
// letzte Änderung:  9.4.2018
// Autor:            Rahm

#ifndef _LCD_H_
#define _LCD_H_

#include "controller.h"

//**** Schalter ********************************************************
#define LCD_LEN 16              // Display-Zeilenlänge 20 oder 16 (default)
#define LCD_I2C                 // Aktiviert das I2C-Interface
#define LCD_WAIT 10              // Bei Timing-Problemen LCD_WAIT vergrößern!!
//**********************************************************************

//Anzeige Einstellungen
#define CURSOR      0x0c        // 0b00001100  ->  Display on, Cursor off, Blink off
#define DISPLAY     0x14        // 0b00010100  ->  Cursor shift right
										
#define ZEILE1      0x00        // Startadresse für erste Displayzeile
#define ZEILE2      0x40

#if (LCD_LEN == 16)
 #define ZEILE3     0x10        // Für 16 Zeichen LCDs Zeile3 = 0x10
 #define ZEILE4     0x50        // "       "       "   Zeile4 = 0x50
#elif (LCD_LEN == 20) 
 #define ZEILE3     0x14        // Für 20 Zeichen LCDs Zeile3 = 0x14
 #define ZEILE4     0x54        // "       "       "   Zeile4 = 0x54
#endif

#ifndef LCD_I2C
 // * |D.7|D.6|D.5|D.4|D.3|D.2|D.1|D.0|    |B.1|B.0|
 // * | 7 | 6 | 5 | 4 | EN| RS| x | x |    | x |R/W|      MyAVR-Board
 // * | 7 | 6 | 5 | 4 | x |R/W| x | x |    |RS |EN |      Carrier-Board
 // * |BSY|

 #define LCD_PORT        PORTD   // LCD-Display an PortD im -4-Bit-Modus
 #define LCD_PORT_CFG    DDRD
 #define LCD_PORT_MASK   0xf0    // Maskiert die 4 Datenbit (4 Bit-Modus)
                                 // Mögliche Werte: f0, 78, 3c, 1e, 0f
 #define LCD_PORT_EN     PORTB   // LCD-Display EN an PortB.0
 #define LCD_PORT_EN_CFG DDRB
 #define EN       0

 #define LCD_PORT_RS     PORTB   // LCD-Display RS an PortB.1
 #define LCD_PORT_RS_CFG DDRB
 #define RS       1

// (D.7) Wenn Busy-Flag nicht abgefragt werden kann/soll,
// dann LCD_BUSY auskommentieren
//#define LCD_BUSY 7              
 #ifdef LCD_BUSY
  #define LCD_PORT_RW     PORTD   // LCD-Display RW an PortB.0
  #define LCD_PORT_RW_CFG DDRD
  #define RW       2
  #define LCD_PORT_BUSY   PIND    // Busy-Flag wird von Bit D7 gelesen!
 #endif

#else   // Für I2C-Display mit PCF8574T (D7..D4 => P7..P4)!!!
 #define RS       0
 #define RW       1
 #define EN       2
 #define BL       3   //Backlight
 //***** I2C-Adressen und Kontrollbyte für den PCF8574 ****************
 #define LCD_ADDR_R  0x4f   // 01001111 ->  0100 fest, 111 durch Jumper, 1 Wert lesen
 #define LCD_ADDR_W  0x4e   // 01001110 ->  0100 fest, 111 durch Jumper, 0 Wert schreiben
#endif

//Funktionsdefinitionen für Technische Richtlinie FA205
extern void lcd_init      ( void );                            // Initialisierung des Displays
extern void lcd_clear     ( void );
extern void lcd_setcursor ( uint8_t row, uint8_t column );     // Setzen der Cursorposition
// row (Zeile) = 1,2, ...
// column (Spalte) = 1,2,3, ...
extern void lcd_print     ( uint8_t  text[] );                 // Textausgabe ab Cursorposition, bis '\0'
extern void lcd_char      ( uint8_t  value  );                 // Ausgabe eines Zeichens an der aktuellen Cursorposition
extern void lcd_byte      ( uint8_t  value  );                 // 0 ... 255
extern void lcd_int       ( uint16_t value  );                 // 0 ... 65535
void        lcd_int32     ( uint32_t value  );                 // 0 ... 99.999.999

//erweiterte Funktionen nicht Teil der Technischen Richtlinie FA205
extern void lcd_defchar   ( uint8_t *pixtab, uint8_t char_nr );
// Definition von 7 eigenen Zeichen. char_nr: 1 bis 7
// char_nr = 0 funktioniert nur, wenn das Zeichen nicht in Zeichenketten ('\0' = 0) verwendet wird.
extern void lcd_clearline ( uint8_t lineNr );
// Ausgabe von 00..99  z.B. für Datum, Uhrzeit
extern void lcd_dd        (uint8_t val);
// Zeigt 0000 bis FFFF auf dem Display an!
extern void lcd_hhhh      (uint16_t val);
// Gibt ein Byte als Bitfolge am LC-Display aus. Z.B.: 01101101
void lcd_debug(uint8_t byte);
#endif
