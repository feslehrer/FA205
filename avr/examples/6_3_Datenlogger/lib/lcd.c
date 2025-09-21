// Technische Richtlinie FA205
// Bibliothek:       lcd.c 
// Controller:       für ATmega 
//                   Mit Support für I2C-Display mit PCF8574
// Version:          2.0
// erstellt am:      17.2.2015
// letzte Änderung:  9.4.2018
// Autor:            Rahm

// Headerdateien inkludieren
#include "lcd.h"

// Deklaration lokale Funktionen
uint8_t lcd_lookup        ( uint8_t ascii );
void    lcd_befehl        ( uint8_t befehl );
void    lcd_daten         ( uint8_t daten );
void    lcd_waitwhilebusy ( void );
void    lowlevel_write    ( uint8_t data );
void    port_out          ( uint8_t wert );

// Definition der Funktionen
// LCD lowlevel-Funktionen: lcd_waitwhilebusy(), lcd_befehl(), lcd_daten() und lcd_init(), lowlevel_write()
// müssen für jeden Controller/Board angepasst werden.
void lcd_waitwhilebusy(void)
{ 
  #ifdef LCD_BUSY
   uint8_t busy;
   LCD_PORT_CFG &= ~LCD_PORT_MASK;          // D.7-D.4 als Eingang
   LCD_PORT_RW |=  (1<<RW);                 // R/W = 1  (Lesen)
   LCD_PORT_RS &= ~(1<<RS);                 // RS = 0   (Lesen Busy+Cursorpos)
   do
   {
     LCD_PORT_EN |=  (1<<EN);               // EN = 1
     LCD_PORT_EN &= ~(1<<EN);               // EN = 0

     busy = LCD_PORT_BUSY & (1<<LCD_BUSY);  // Read Busy Flag (und Address High Nibble (Pin!))
     LCD_PORT_EN |=  (1<<EN);               // EN = 1 Read Address Low Nibble (dummy)
     LCD_PORT_EN &= ~(1<<EN);               // EN = 0
   } while (busy != 0);

   LCD_PORT_RW &= ~(1<<RW);                 // R/W(B.0) = 0  (Schreiben)
   LCD_PORT_RS |=  (1<<RS);                 // RS = 1   (Lesen Daten)
   LCD_PORT_CFG  |=  LCD_PORT_MASK;         // D.7-D.4 wieder als Ausgang 
  #else  // Wenn kein Busy-Flag abgefragt wird!!
   delay_100us(LCD_WAIT);
  #endif
}

#ifdef LCD_I2C
// 8-Bit Portausgabe (PCF8574) über I2C-Bus
void port_out(uint8_t wert)
{
  i2c_start();                // Startbedingung    
  i2c_write(LCD_ADDR_W);      // Schreibwunsch an Port-Expander senden
  //delay_100us(2);
  i2c_write(wert);            // port0 ausgeben
  i2c_stop();                 // Stoppbedingung
}
#endif

void lowlevel_write(uint8_t data)
{
#ifdef LCD_I2C
  //data |=  (1<<BL);             // BL = on (Backlight)
  data |=  (1<<EN);             // EN = 1
  port_out(data);
  data &= ~(1<<EN);             // EN = 0
  port_out(data);
#else
  LCD_PORT &= ~LCD_PORT_MASK;
  LCD_PORT |= data;
  //delay_100us(2);
  LCD_PORT_EN |= (1<<EN);       // EN = 1
  LCD_PORT_EN &= ~(1<<EN);      // EN = 0
#endif
}

/*****************************************************************************************
 * 	Gibt das Byte 'befehl' als Befehl ans LCD-Display im 4Bit-Modus                              
 *     Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden        
 *****************************************************************************************/
#if (LCD_PORT_MASK == 0xf0)
#define MSB_SHIFT 0
#define LSB_SHIFT 4
#endif
#if (LCD_PORT_MASK == 0x78)
#define MSB_SHIFT 1
#define LSB_SHIFT 3
#endif
#if (LCD_PORT_MASK == 0x3c)
#define MSB_SHIFT 2
#define LSB_SHIFT 2
#endif
#if (LCD_PORT_MASK == 0x1e)
#define MSB_SHIFT 3
#define LSB_SHIFT 1
#endif
#if (LCD_PORT_MASK == 0x0f)
#define MSB_SHIFT 4
#define LSB_SHIFT 0
#endif

void lcd_befehl (uint8_t befehl)
{
   uint8_t temp;

   lcd_waitwhilebusy();                     // Warten bis LCD bereit!

  #ifdef LCD_I2C		
   temp = befehl;
   temp &=  0xf0;                           // High-Nibble   	
   temp &= ~(1<<RS);                       // RS = 0  (Befehl)
   lowlevel_write(temp);

   temp = befehl;                           // Low-Nibble
   temp &= 0x0f;
   temp <<= 4;
   temp &= ~(1<<RS);                       // RS = 0  (Befehl)
   lowlevel_write(temp);
  #else
   temp = befehl;
   temp &=  0xf0;                           // High-Nibble
   temp >>= MSB_SHIFT;
   LCD_PORT_RS &= ~(1<<RS);                  // Data-Register On
   lowlevel_write(temp);

   temp = befehl;                           // Low-Nibble
   temp &= 0x0f;
   temp <<= LSB_SHIFT;
   lowlevel_write(temp);
  #endif
}

/*****************************************************************************************
 * 	Gibt das Byte 'daten' ans LCD-Display im 4Bit-Modus aus.
 *    Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden        
 *****************************************************************************************/
void lcd_daten(uint8_t daten)
{
  uint8_t temp;
	
  lcd_waitwhilebusy();                     // Warten bis LCD bereit!
  #ifdef LCD_I2C	
   temp = daten ;
   temp &=  0xf0;                           // High-Nibble
 
   temp |=  (1<<RS);                       // RS = 1  (Daten)
   lowlevel_write(temp);

   temp = daten;                            // Low-Nibbel
   temp &= 0x0f;

   temp <<= 4;
   temp |=  (1<<RS);                       // RS = 1  (Daten)
   lowlevel_write(temp);
  #else
   temp = daten;
   temp &=  0xf0;                           // High-Nibble
   temp >>= MSB_SHIFT;
   LCD_PORT_RS |= (1<<RS);                     // Data-Register On
   lowlevel_write(temp);

   temp = daten;                            // Low-Nibbel
   temp &= 0x0f;
   temp <<= LSB_SHIFT;
   LCD_PORT_RS |= (1<<RS);                  // Data-Register On
   lowlevel_write(temp);
   LCD_PORT_RS &= ~(1<<RS);                 // Data-Register Off
  #endif
}

/******************************************************************************************
 * Initialisierung des LCD- Displays für den 4-Bit-Modus                                         
 ******************************************************************************************/
void lcd_init (void)	
{ 
  uint8_t temp;

  #ifdef LCD_I2C
   delay_ms(LCD_WAIT*6);	
   temp = 0x30;
   i2c_init();
   temp &= ~(1<<RS);                       // RS = 0  (Befehl)
   temp &= ~(1<<RW);                       // R/W = 0 (schreiben!!)
   lowlevel_write(temp);                   // 1. aufwecken!
	
   delay_ms(LCD_WAIT*2);	
   lowlevel_write(temp);                   // 2. aufwecken!

   delay_ms(LCD_WAIT);
   lowlevel_write(temp);                   // 3. aufwecken!

   delay_ms(LCD_WAIT);
   temp = 0x20;
   temp &= ~(1<<RS);                       // RS = 0  (Befehl)
   temp &= ~(1<<RW);                       // R/W = 0 (schreiben!!)
   lowlevel_write(temp);
  #else  // Display im 4-Bit-Mode am ATmega Port
   LCD_PORT_CFG     |=   LCD_PORT_MASK;    // D.7-D.4 als Ausgang (4 Datenleitungen)
   LCD_PORT_EN_CFG  |=  (1<<EN);           // D.3 als Ausgang ( = 1)
   LCD_PORT_RS_CFG  |=  (1<<RS);           // D.2 als Ausgang
   #ifdef LCD_BUSY
    LCD_PORT_RW_CFG  |=  (1<<RW);          // B.0 als Ausgang
    LCD_PORT_RW &= ~(1<<RW);               // R/W(B.0) = 0 (schreiben!!)
   #endif
   delay_ms(LCD_WAIT*6);
   temp = 0x30;
   temp &=  0xf0;                          // High-Nibble
   temp >>= MSB_SHIFT;
   lowlevel_write(temp);                   // 1. aufwecken!
  
   delay_ms(LCD_WAIT*2);
   lowlevel_write(temp);                   // 2. aufwecken!
  
   delay_ms(LCD_WAIT);
   lowlevel_write(temp);                   // 3. aufwecken!

   delay_ms(LCD_WAIT);
   temp = 0x20;
   temp &=  0xf0;
   temp >>= MSB_SHIFT;
   lowlevel_write(temp);
  #endif
  // Ab hier Busy-Flag Abfrage möglich	
  lcd_befehl (0x28);                       // Function set 4 bits  													
  lcd_befehl (CURSOR);                     // Display AN, Cursor AUS													
  lcd_befehl (DISPLAY);                    // Not Shifted Display, Increment				
}

// Ab hier sind alle Funktionen Controllerunabhängig!!!!
/******************************************************************************************
 * Display löschen	                                                                                                      
 ******************************************************************************************/
void lcd_clear (void)     { lcd_befehl(0x01); }

/*****************************************************************************************
 * LCD-Cursor auf eine position setzen: Zeile 1..4, Spalte 1..16 (Displayabhängig)               
 ******************************************************************************************/
void lcd_setcursor (uint8_t zeile, uint8_t spalte)
{
  uint8_t position;
	
  switch(zeile)
  {
    case 1:   position = ZEILE1; break;
    case 2:   position = ZEILE2; break;	
    case 3:   position = ZEILE3; break;
    case 4:   position = ZEILE4; break;
    default:  position = ZEILE1; break;		
  }
	
  position += (spalte-1);
		
  lcd_befehl ( position | 0x80 );          // 0x80 = Kennung für DD RAM address set
}								

/*****************************************************************************************
 * 	Ausgabe eines Zeichens an das LCD-Display                                                          
 *  Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden         
 ******************************************************************************************/
void lcd_char (uint8_t zeichen)
{
  zeichen = lcd_lookup(zeichen);           // Umlaute und Sonderzeichen ersetzen!
  lcd_daten(zeichen);
}

/*****************************************************************************************
 * \0-terminierten Text an das LCD-Display ausgeben. 
 ******************************************************************************************/
void lcd_print (uint8_t text[])
{				
  while (*text != '\0')                   // Text													
    lcd_char (*text++);                   // zeichenweise ausgeben 	
}

/*============================================================
Funktion:        lcd_byte(n)                Rahm, 17.2.15
Beschreibung:    Gibt das Byte n als 3 stelligen Dez-Wert aufs
                 Display. Führende Nullen werden zu blank.
Eingang:         Byte
Ausgang:         ---
==============================================================*/
void lcd_byte(uint8_t val)
{
  uint8_t buffer[3];
  uint8_t n = 0;	
	
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
				
  while (n<3)                             // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';					
  }

  while (n > 0)                           // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    lcd_char(buffer[n]);
  }
}

/*============================================================
Funktion:        lcd_int(n)                Rahm, 17.2.15
Beschreibung:    Gibt den Integer n als 5 stelligen Dez-Wert aufs
                 Display. Führende Nullen werden zu blank.
Eingang:         Byte
Ausgang:         ---
==============================================================*/
void lcd_int(uint16_t val)
{	
  uint8_t buffer[5];
  uint8_t n = 0;	
	
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
		
  while (n<5)                   // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';					
  }

  while (n > 0)                 // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    lcd_char(buffer[n]);
  }
}

/*============================================================
Funktion:        lcd_int32(n,allign)                Rahm, 19.6.24
Beschreibung:    Gibt den Integer n (0...99.999.999) als 8 stelligen
                 Dez-Wert aufs Display aus.
                 allign = 0 = _TEXT_ALLIGN_RIGHT_ : Führende Nullen werden zu blank.
                 allign = 1 = _TEXT_ALLIGN_LEFT_  : Linksbündige Ausgabe
Eingang:         int32, uint8
Ausgang:         ---
==============================================================*/
void lcd_int32(uint32_t val, uint8_t allign)
{	
  uint8_t buffer[8];
  uint8_t n = 0;	
  
  if(val>99999999L) 
  {
    lcd_print("err >max");  // Fehler 
    return;
  }
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
		
  while (n<8)                   // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';
  }

  while (n > 0)                 // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    if (allign==_TEXT_ALLIGN_LEFT_ && buffer[n]!=' ')
      lcd_char(buffer[n]);
    if (allign==_TEXT_ALLIGN_RIGHT_)
      lcd_char(buffer[n]);
  }
}

/*============================================================
Funktion:     LookUpTable(char)                Rahm, 4.6.09
Beschreibung: Ersetzt ein ASCII-Umlaut durch den ent-
              sprechenden Display-Code.
Eingang:      ASCII-Code
Ausgang:      Display-Code
==============================================================*/
uint8_t lcd_lookup(uint8_t ascii)
{
  switch (ascii)
  {
    case 0xb0: return 0xdf;  // '°'
    case 'ä':
    case 'Ä':  return 0xe1;
    case 'ö':
    case 'Ö':  return 0xef;
    case 'ü':
    case 'Ü':  return 0xf5;
    case 'ß':  return 0xe2;
    case 'µ':  return 0xe4;
    case '\\': return 0xa4;
    case '':  return 0xd3;
  }
  return ascii;
}

/////////////////////////////////////////////////////////////////////////////////
// Ab hier sind ergänzende Funktionen definiert, die nicht Teil der technischen
// Richtlinie FA205 sind.
/////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * Pixelweise Definition eigener LCD-Zeichen, Ablegen von max 7 Zeichen im CG-RAM des LCD
 ***********************************************************************************/
void lcd_defchar (uint8_t *pix_tab, uint8_t char_nr)
{
  uint8_t i;
	
  if (char_nr<8)                            // Adr 0 nicht innerhalb eines Textstrings nutzbar
  lcd_befehl (((char_nr*8) & 0x7F) | 0x40);	
  // Adresse der ersten Pixelzeile = 8 x Adresse des fertigen Zeichens im DD Ram
  // Zugriff auf Adresse im CG-RAM (Character Generator) mit  Bit7 = 0, Bit6 = 1
  for (i=0;i<8;i++)	
    lcd_char (*(pix_tab+i));          // 8 Pixelzeilen ins CR-Ram
  lcd_setcursor (1,1);                        // wieder auf DD-Ram-Zugriff umschalten		
}

/*************************************************************************
Löschen der Zeilen 1 bis 4 des Displays
Es wird ein Leerstring (16 Byte) auf die Zeile geschrieben
***************************************************************************/
void lcd_clearline(uint8_t line)
{
  uint8_t n;

  lcd_setcursor(line,0);
  for (n=0; n < LCD_LEN; n++)  lcd_char(' ');
}

// Zeigt 00 bis 99 auf dem Display an!
void lcd_dd(uint8_t val)
{
	uint8_t buffer[3];
	uint8_t n = 0;
	
	do
	{
		buffer[n++] = val%10 + '0';
		val /= 10;
	} while (n<3);

	n = 2;				// Nur 2 Stellen anzeigen
	while (n > 0)   
	{
		n--;
		lcd_char(buffer[n]);
	}
}

/*************************************************************************
Gibt ein Byte als Bitfolge am LC-Display aus
***************************************************************************/
void lcd_debug(uint8_t byte)
{
  for(int8_t i=7; i>=0;i-- )
  {
    if (((byte>>i) & 0x01) == 0x01)  lcd_char('1');
    else lcd_char('0');
  }
}

// Zeigt 0000 bis FFFF auf dem Display an!
void lcd_hhhh(uint16_t val)
{
  uint8_t buffer[5];
  uint8_t temp;
  uint8_t n = 0;
  
  do
  {
    temp = val & 0x0f;
    if (temp < 0x0A)  temp += '0';
    else
    {
      temp -= 10;
      temp += 'A';
    }
    buffer[n++] = temp;
    val >>= 4;
  } while (n<4);

  n = 4;				// 4 Stellen anzeigen
  while (n > 0)
  {
    n--;
    lcd_char(buffer[n]);
  }
}
