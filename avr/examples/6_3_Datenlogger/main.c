/*
 Beschreibung:			Datenlogger mit DS1307 und 24LCxx
   
 Erstellt am:       26.10.2017
 Letzte Änderung:   24.03.2018
 
 Autor:							Rahm
*/

#include "controller.h"
#include "datalogger.h"

// Display-Zeichen für Sendeanzeige
uint8_t zeichen1[] = {  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
uint8_t zeichen2[] = {  0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00 };
uint8_t zeichen3[] = {  0x00,0x00,0x08,0x04,0x14,0x04,0x08,0x00 };
uint8_t zeichen4[] = {  0x04,0x02,0x09,0x05,0x15,0x05,0x09,0x02 };
// Funktions-Prototypen
void     lcd_print_timestamp(void);
// Globale Variablen
volatile uint16_t _SAMPLE_TIME_ = 120;			  // Ein Sample alle 120s (=2min)
volatile uint8_t  temp_anz = 0;               // Merker für Fortlaufende Temperaturanzeige rs232
volatile uint8_t  anz_mode = 0;               // Merker für Anzeigemodus (plain, name)
//*******************************************************************
void setup(void)
{  // Initialisierungen
  lcd_init(); 
	i2c_init();
	rtc_init();
	eeprom_init();
  rs232_init();
  serial_interrupt_init(serial_interrupt_isr);
  
  // Definition der Sendezeichen
  lcd_defchar(zeichen1,1);
  lcd_defchar(zeichen2,2);
  lcd_defchar(zeichen3,3);
  lcd_defchar(zeichen4,4);
  //-Startbildschirm-------------------------  
	lcd_clear();
	lcd_setcursor(1,1);
	lcd_print("FES Datenlogger");
	lcd_setcursor(2,1);
	lcd_print("COM: 9600Bd");
  delay_ms(4000);
	lcd_clear();
  //--------------------------
  aktueller_Datensatz = read_current_recordnumber_from_rtc();
	aktueller_Datensatz++;		// Damit die letzten Daten nicht überschrieben werden.
  _SAMPLE_TIME_ = read_current_sampletime_from_rtc();
  if(_SAMPLE_TIME_ < 60) {_SAMPLE_TIME_ = 60; write_current_sampletime_to_rtc(_SAMPLE_TIME_);}
}
//*******************************************************************
int main(void)
{
	volatile uint16_t sample_sekunde;
  uint8_t n=1;
  
  setup();
	
	serial_interrupt_enable();

  while (1)      // Endlosschleife loop()
  { 
		temperatur = lm75_read();
		rtc_get();
		sample_sekunde = sekunde + minute * 60 + stunde * 60*60;
		if((sample_sekunde % _SAMPLE_TIME_) == 0)
		{
      if(aktueller_Datensatz >= MAX_RECORD) aktueller_Datensatz = 0; // Neubeginn der Aufzeichnung
      write_current_recordnumber_to_rtc(aktueller_Datensatz);
			eeprom_set_record(EEPROM_1,aktueller_Datensatz++);
		}
	  lcd_print_timestamp();
		lcd_setcursor(2,1);
    lcd_int(aktueller_Datensatz); lcd_char(':');
    lcd_setcursor(2,8);
		lcd_print_temperatur(temperatur);   // Ausgabe
    lcd_setcursor(2,16);                // Sendeanzeige
    if(temp_anz)
    {
      rs232_print_temperatur(temperatur,anz_mode);
      lcd_char(n++);
      if (n>4) n = 1;
    }
    else lcd_char(' ');    
		delay_ms(1000);
  }
}
//*******************************************************************
void serial_interrupt_isr(void)
{
  uint8_t c;
  uint32_t n;
  int8_t test = 0;
  
  c=rs232_get();
  
  switch (c)
  {
    case 'f':
    case 'F': //Toggle Temperaturanzeige
              if (temp_anz == 0) temp_anz = 1;
              else               temp_anz = 0;
              break;
    case 'm':
    case 'M': //Toggle Anzeigemodus (plain, name)
              switch (anz_mode)
              {
                case 0: anz_mode = 1;break;
                case 1: anz_mode = 2;break;
                case 2: anz_mode = 0;break;
                default: break;
              }              
              break;
    case 't':
    case 'T': // Zeit einstellen
              rs232_set_time();
              rs232_put('\n');
              break;
    case 'p':
    case 'P': // alle Datensätze drucken
              serial_print_all_records();
              break;
    case 'n':  
    case 'N': // Aufzeichnung neustarten
              rs232_print("Alle Datensätze werden gelöscht [j/n]: ");
              while((c=rs232_get())== 0);
              rs232_put(c);
              if ((c=='j')||(c=='J'))  
              {
                aktueller_Datensatz = 0;
                write_current_recordnumber_to_rtc(aktueller_Datensatz);
              }                
              rs232_put('\n');
              break;
    case 's':
    case 'S': // Sample-Zeit ändern
              rs232_print("\nSample-Zeit in s[");rs232_int(_SAMPLE_TIME_);rs232_print("]: ");
              if (((n = rs232_get_sampletime()) >= 60)&&(n<=0xffff)) 
              {
                _SAMPLE_TIME_ = n;
                write_current_sampletime_to_rtc(_SAMPLE_TIME_);
              }              
              else rs232_print("60s <= Sampeltime <= 65535s !!!\n");            
              break;
    case 'e':
    case 'E': // EEPROM Speichertest
              lcd_clear();
              lcd_setcursor(1,1);
              lcd_print("EEPROM #1 test");
              test = eeprom_memtest(EEPROM_1);
              if (test == -1) {lcd_setcursor(1,1); lcd_print("!Speicherfehler!");while(1);}              
              break;
    case 'h':
    case 'H': // Hilfe anzeigen
              rs232_print("\nHilfe:\n");
              rs232_print("t: Datum und Zeit einstellen\n");
              rs232_print("f: Temperatur fortlaufend ausgeben\n   [m: Umschalten des Anzeige-Modus]\n");              
              rs232_print("p: Alle Datensätze drucken\n");
              rs232_print("n: Aufzeichnung neustarten\n");
              rs232_print("s: Sample-Zeit einstellen [default 120s]\n");
              rs232_print("e: EEPROM Speichertest\n"); 
              break;
    default:  break;           
  }
}
//*******************************************************************
void lcd_print_timestamp(void)
{
	lcd_setcursor(1,1);
	lcd_dd(tag); lcd_char('.'); lcd_dd(monat); lcd_char('.');	lcd_dd(jahr);
  lcd_print("   "); lcd_dd(stunde); lcd_char(':'); lcd_dd(minute);
}
//*******************************************************************