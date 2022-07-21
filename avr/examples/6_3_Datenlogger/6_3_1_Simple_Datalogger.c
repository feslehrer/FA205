/*
 Beschreibung:			Datenlogger mit DS1307 und 24LCxx
   
 Erstellt am:       26.10.2017
 Letzte Änderung:   29.03.2018
 
 Autor:							Rahm
*/

#include "controller.h"
#include "datalogger.h"

// Funktions-Prototypen
void     lcd_print_timestamp(void);
// Globale Variablen
volatile uint16_t _SAMPLE_TIME_ = 120;			  // Ein Sample alle 120s (=2min)

//*******************************************************************
void setup(void)
{  // Initialisierungen
  lcd_init(); 
	i2c_init();
	rtc_init();
	eeprom_init();
  rs232_init();
  serial_interrupt_init(serial_interrupt_isr);

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
		rtc_get();                        // Aktuelles Datum und Zeit holen.
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
    rs232_print_temperatur(temperatur,2); 
		delay_ms(1000);
  }
}
//*******************************************************************
void serial_interrupt_isr(void)
{
  uint8_t c;
 
  c=rs232_get();
  
  switch (c)
  {
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
    case 'h':
    case 'H': // Hilfe anzeigen
              rs232_print("\nHilfe:\n");
              rs232_print("t: Datum und Zeit einstellen\n");
              rs232_print("p: Alle Datensätze drucken\n");
              rs232_print("n: Aufzeichnung neustarten\n");
              rs232_print("s: Sample-Zeit einstellen [default 120s]\n");
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