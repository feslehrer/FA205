/* 
  Beschreibung:     Zusatzfunktionen f�r Datenlogger-Projekt

  Autor:            Rolf Rahm
  Datum:            01.11.2017
  Letzte �nderung:  11.12.2021
*/
#include "controller.h"
#include "datalogger.h"
// Globale Variablen in datalogger.h
// tag,monat,jahr,stunde,minute,sekunde,temperatur

//***************************************************************************
//**** Datenlogger-Funktionen ************************************************
//***************************************************************************
void rs232_print_record(uint8_t i2c_address, uint16_t record)
{
	uint8_t dezimale;
	
	eeprom_get_record(EEPROM_1,record);
	rs232_printdd(tag);     rs232_put('.');
	rs232_printdd(monat);   rs232_put('.');
	rs232_printdd(jahr);    rs232_put(' ');
	rs232_printdd(stunde);  rs232_put(':');
	rs232_printdd(minute);  rs232_put(':');
	rs232_printdd(sekunde); rs232_put(';');
	if ((temperatur & 0x80) == 0x80)  dezimale = '5';
	else                              dezimale = '0';

	if ( temperatur < 0 )        // Temperatur negativ?
	{
		temperatur = ~temperatur;
		temperatur++;             // 2er-Komplement
		rs232_put('-');
	}
	rs232_printdd((uint8_t)(temperatur>>8)&0xff); rs232_put(',');
	rs232_put(dezimale);
	rs232_put('\n');
}
//---------------------------------------------------------------------------
void rs232_set_time(void)
{
  uint8_t temp,i;

  rs232_print("Uhr stellen [j/n] ?: ");
  temp = 'n';
  for (i=6;i>0;i--)
  {
    rs232_put(i-1+'0');
    rs232_put('\b');
    delay_ms(1000);
    temp = rs232_get();
    if (temp!='\0')  break;
  }

  rs232_put(temp);            // Echo
  if ((temp == 'j') || (temp=='J'))
  {
    rs232_print("\n***** Aktuelles Datum *****");

    rs232_print("\nTag     [01..31]: ");
    tag = rs232_inputdd();

    rs232_print("\nMonat   [01..12]: ");
    monat = rs232_inputdd();

    rs232_print("\nJahr    [00..99]: ");
    jahr = rs232_inputdd();

    rs232_print("\nAktuelle Uhrzeit:");

    rs232_print("\nStunde  [01..24]: ");
    stunde = rs232_inputdd();

    rs232_print("\nMinuten [00..59]: ");
    minute = rs232_inputdd();

    sekunde = 00;
    rtc_set();
  }
}
//---------------------------------------------------------------------------
void eeprom_get_record(uint8_t i2c_address, uint16_t record)
{
	volatile uint16_t msb,lsb;
	
	jahr =     eeprom_read(i2c_address,record*_REC_SIZE_);
	monat =    eeprom_read(i2c_address,record*_REC_SIZE_+1);
	tag =      eeprom_read(i2c_address,record*_REC_SIZE_+2);
	stunde =   eeprom_read(i2c_address,record*_REC_SIZE_+3);
	minute =   eeprom_read(i2c_address,record*_REC_SIZE_+4);
	sekunde =  eeprom_read(i2c_address,record*_REC_SIZE_+5);
	msb =      eeprom_read(i2c_address,record*_REC_SIZE_+6);
	lsb =      eeprom_read(i2c_address,record*_REC_SIZE_+7);
	temperatur = (msb<<8) | lsb ;
}
//---------------------------------------------------------------------------
void eeprom_set_record(uint8_t i2c_address, uint16_t record)
{
	volatile uint8_t test;
	eeprom_write(i2c_address,record*_REC_SIZE_  ,jahr);
	eeprom_write(i2c_address,record*_REC_SIZE_+1,monat);
	eeprom_write(i2c_address,record*_REC_SIZE_+2,tag);
	eeprom_write(i2c_address,record*_REC_SIZE_+3,stunde);
	eeprom_write(i2c_address,record*_REC_SIZE_+4,minute);
	eeprom_write(i2c_address,record*_REC_SIZE_+5,sekunde);
	test = (uint8_t)(temperatur >> 8);
	eeprom_write(i2c_address,record*_REC_SIZE_+6,test);   //msb
	test = (uint8_t)(temperatur & 0xff);
	eeprom_write(i2c_address,record*_REC_SIZE_+7,test); //lsb
}
//---------------------------------------------------------------------------
void rtc_set()
{
  rtc_write(_YEAR_,jahr);
  rtc_write(_MONTH_,monat);
  rtc_write(_DATE_,tag);
  rtc_write(_HR_,stunde);
  rtc_write(_MIN_,minute);
  rtc_write(_SEC_,sekunde);	
}
//---------------------------------------------------------------------------
void rtc_get()
{
	jahr    = rtc_read(_YEAR_);
	monat   = rtc_read(_MONTH_);
	tag     = rtc_read(_DATE_);
	stunde  = rtc_read(_HR_);
	minute  = rtc_read(_MIN_);
	sekunde = rtc_read(_SEC_);
}
//----------------------------------------------------------------------------
uint16_t read_current_recordnumber_from_rtc(void)
{ uint16_t temp;
	temp = rtc_lowlevel_read(RECORD_MSB);		 //msb
	temp <<=8;
	temp |= rtc_lowlevel_read(RECORD_LSB);    //lsb
	return temp;
}
//-----------------------------------------------------------------------------
void write_current_recordnumber_to_rtc(uint16_t record_number)
{
	rtc_lowlevel_write(RECORD_MSB,(uint8_t)(record_number>>8) & 0xff);
	rtc_lowlevel_write(RECORD_LSB,(uint8_t) record_number     & 0xff);
}
//----------------------------------------------------------------------------
uint16_t read_current_sampletime_from_rtc(void)
{ uint16_t temp;
  temp = rtc_lowlevel_read(SAMPLETIME_MSB);		 //msb
  temp <<=8;
  temp |= rtc_lowlevel_read(SAMPLETIME_LSB);    //lsb
  return temp;
}
//-----------------------------------------------------------------------------
void write_current_sampletime_to_rtc(uint16_t sample_time)
{
  rtc_lowlevel_write(SAMPLETIME_MSB,(uint8_t)(sample_time>>8) & 0xff);
  rtc_lowlevel_write(SAMPLETIME_LSB,(uint8_t) sample_time     & 0xff);
}
//-----------------------------------------------------------------------------
void serial_print_all_records(void)
{
  uint16_t i;
  uint8_t dezimale;
  int8_t ganzzahl;
  uint8_t buffer[6];

  delay_ms(20);
  
  for (i=0;i<aktueller_Datensatz;i++)
  {
    eeprom_get_record(EEPROM_1,i);
    rs232_printdd(tag);     rs232_put('.');
    rs232_printdd(monat);   rs232_put('.');
    rs232_printdd(jahr);    rs232_put(' ');
    rs232_printdd(stunde);  rs232_put(':');
    rs232_printdd(minute);  rs232_put(':');
    rs232_printdd(sekunde); rs232_put(';');
    // Nachkommastelle. Aufl�sung 0,5�C
    dezimale = temperatur;
    if (dezimale!=0) dezimale = 5; else dezimale = 0;
    ganzzahl = temperatur >>8;
    sprintf(buffer,"%3d,%1u\n",ganzzahl,dezimale);
    rs232_print(buffer);
    //rs232_put('\n');
  }
}
//-----------------------------------------------------------------------------
void eeprom_speichertest(void)
{ 
  int8_t test = 0;
	
  lcd_setcursor(1,1);
	lcd_print("EEPROM #1 test");
	test = eeprom_memtest(EEPROM_1);
  if (test == -1) 
  {
    lcd_setcursor(1,1);
    lcd_print("!Speicherfehler!");
  }    
  
  //
	//lcd_setcursor(1,1);
	//lcd_print("EEPROM #2 test");
	//test = eeprom_memtest(EEPROM_2);
	//lcd_setcursor(1,1);
	//lcd_print("EEPROM #3 test");
	//test = eeprom_memtest(EEPROM_3);
}
//---------------------------------------------------------------------------
void lcd_print_temperatur(int16_t degree)
{
	uint8_t  buffer[LCD_LEN+1];
	uint8_t  dezimale,ganzzahl;

	// Nachkommastelle. Aufl�sung 0,5�C
  dezimale = degree & 0x80;   //nur Bit lsb:  0000 0000 1000 0000
  if (dezimale!=0) dezimale = 5; else dezimale = 0;
  ganzzahl = degree >>8;
    
  // Zusammenbauen der Zeichenkette mit der Bibliotheksfunktion sprintf()
  sprintf(buffer,"%3d,%1u�C",ganzzahl,dezimale);
  
	// Ausgabe auf LC-Display
	lcd_print(buffer);
}
//---------------------------------------------------------------------------
void rs232_print_temperatur(int16_t degree, uint8_t mode)
{
	uint8_t  buffer[LCD_LEN+1];
	uint8_t  dezimale,ganzzahl;

	// Nachkommastelle. Aufl�sung 0,5�C
  dezimale = degree & 0x80;   //nur Bit lsb:  0000 0000 1000 0000
  if (dezimale!=0) dezimale = 5; else dezimale = 0;
  ganzzahl = degree >>8;
  
  // Zusammenbauen der Zeichenkette mit der Bibliotheksfunktion sprintf()
  //sprintf(buffer,"%3d,%1u�C",ganzzahl,dezimale);
  switch (mode)
  {
    case 0:    sprintf(buffer,"T1=%3d.%1u\r\n",ganzzahl,dezimale); break;
    
    case 1:    sprintf(buffer,"%3d.%1u\r",ganzzahl,dezimale); break;
    
    case 2:    sprintf(buffer,"%3d.%1u�C\r",ganzzahl,dezimale); break;
    default: break;
  }  
	// Ausgabe auf LC-Display
	rs232_print(buffer);           //rs232_put('\n');
}
//---------------------------------------------------------------------------
int16_t lm75_read(void)
{
  uint16_t msb,lsb,data16;
  
  i2c_start();                // Startbedingung
  i2c_write(LM75_ADDR_W);     // Schreibwunsch an LM75 senden
  i2c_write(0x00);            // Pointer auf 0 setzen
  i2c_stop();
  
  i2c_start();
  i2c_write(LM75_ADDR_R);     // Lesewunsch an LM75 senden
  msb = i2c_read(ACK);        // MSB lesen und weiteren Wert anfordern.
  lsb = i2c_read(NACK);       // LSB lesen
  i2c_stop();                 // Stoppbedingung
  
  msb <<= 8;
  data16 = msb | lsb;
  data16 &= 0xff80;           // niederwertige Bits ausmaskieren! 
  return data16;              // Genauigkeit: +/- 0,5�C
}
//---------------------------------------------------------------------------
uint32_t rs232_get_sampletime(void)
{
  volatile uint8_t c,i;
  volatile uint32_t buf;

  buf = 0, i = 0;
  
  do
  {
    while((c = rs232_get()) == 0);
    if(c == '\n' || c == '\r')
    break;
    else if ((c >= '0') && (c <= '9'))
    {
      buf *= 10;
      buf += (c-'0');
      i++;
      rs232_put(c);       // Echo
    }
  } while (i<5);
  
  rs232_put('\n');
  return buf;
}