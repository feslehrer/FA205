/* 
  Beschreibung: Real-Time-Clock (DS1307) mit i2c-Bus-Interface
								Low-Level-Funktionen
								
  Autor:            Rolf Rahm
  Datum:            23.10.2017
  Letzte Änderung:  9.4.2018
*/
#include "controller.h"
#include "rtc.h"

void rtc_init(void)
{
	uint8_t temp;
  i2c_init();			   							// Zugriff auf RTC über i2C-Bus
	temp = rtc_lowlevel_read(_SEC_);
	rtc_lowlevel_write(_SEC_,temp & 0x7F);		// ClockHalt(CH)-Bit 0 --> Clock startet
}

// Adressdefinitionen für Zugriff auf die Timekeeper-Register
// in rtc.h
/*
#define _SEC_			  0x00
#define _MIN_       0x01
#define _HR_        0x02
#define _DAY_       0x03
#define _DATE_      0x04
#define _MONTH_     0x05
#define _YEAR_      0x06
*/
//-Lesen der Timekeeper-Register--------------------------
uint8_t rtc_read (uint8_t reg)
{
    uint8_t temp;
		
		temp = rtc_lowlevel_read( reg );
		
    temp = (temp & 0x0f) + ((temp & 0x70)>>4)*10;
		
    return temp;
}
//-Schreiben der Timekeeper-Register----------------------
void rtc_write(uint8_t reg, uint8_t value)
{
	value = ((value/10)<<4) | (value%10);
	
	rtc_lowlevel_write(reg,value);
}
//-Lesen aus internem Ram des RTC-------------------------
uint8_t rtc_lowlevel_read (uint8_t reg)
{
    uint8_t temp;

    i2c_start();
    i2c_write(RTC_ADDR_W);
    i2c_write(reg);

    i2c_start();  // Repeated Start
		i2c_write(RTC_ADDR_R);
    temp = i2c_read(NACK);
    i2c_stop();
		
    return temp;
}
//-Schreiben ins interne RAM des RTC----------------------
void rtc_lowlevel_write(uint8_t reg, uint8_t value)
{   
   i2c_start();
   i2c_write(RTC_ADDR_W);
   i2c_write(reg);
   
   i2c_write(value);
   i2c_stop();
}

