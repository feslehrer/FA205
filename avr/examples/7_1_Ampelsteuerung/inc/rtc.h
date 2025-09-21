/*
  Header: Real-Time-Clock (DS1307) mit i2c-Bus-Interface
          Low-Level-Funktionen
  
  Autor:            Rolf Rahm
  Datum:            23.10.2017
  Letzte Änderung:  9.4.2018
*/
#ifndef _RTC_H
#define _RTC_H

#include "controller.h"

// i2c-Adressen
#define  RTC_ADDR_R  0xd1
#define  RTC_ADDR_W  0xd0

// Adressen der Timekeeper-Register DS1307
#define _SEC_			  0x00
#define _MIN_       0x01
#define _HR_        0x02
#define _DAY_       0x03
#define _DATE_      0x04
#define _MONTH_     0x05
#define _YEAR_      0x06

// Funktionsprototypen für Echtzeituhr
// Initialisierung
void     rtc_init           (void);
// Schreiben der Timekeeperregister (BCD-Format)
// reg   =     _SEC_, _MIN, _HR_, _DAY, _DATE_, _MONTH_, _YEAR_
// value = 00...59  ,  59,   23 ,  31 ,   7   ,   12   ,   99
void     rtc_write          (uint8_t reg, uint8_t value);
// Lesen der Timekeeper-Register (BCD-Format)
uint8_t  rtc_read           (uint8_t reg);
// Byteweises Schreiben ins interne RAM (Binär-Format).
// Z.B. Speichern von batteriegepufferten Werten.
// reg   =  0x08....0x3F, 0x00..0x07 (=Timekeeper-Register!!!)
// value =  0x00...0xff
void     rtc_lowlevel_write (uint8_t reg, uint8_t value);
//Byteweises Lesen aus dem internen RAM
uint8_t	 rtc_lowlevel_read  (uint8_t reg);

#endif

