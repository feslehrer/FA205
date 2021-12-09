/*  
	Header:           Datenlogger-Funktionen
  
  Autor:            Rolf Rahm
  Datum:            23.10.2017
  Letzte Änderung:  9.4.2018
*/
#ifndef _DATALOGGER_H
#define _DATALOGGER_H

#include "controller.h"
#include "eeprom.h"
#include "rtc.h"

// Globale Variablen als Übergabe-Parameter für das Datenlogger-Programm (main)
volatile uint8_t jahr,monat,tag,stunde,minute,sekunde;
volatile int16_t temperatur;
volatile uint16_t aktueller_Datensatz;			// -> im gewählten EEPROM

// i2c-Adressen für Temperatursensor
#define  LM75_ADDR_R  0x91
#define  LM75_ADDR_W  0x90

#define _REC_SIZE_		8				    // Bytes je Datensatz
#define RECORD_MSB  0x08					// Speicheradressen für aktuellen Datensatz 
#define RECORD_LSB  0x09					// wird im RTC-Ram batteriegepuffert
#define SAMPLETIME_MSB  0x0A			// Speicheradressen für Sample-Time
#define SAMPLETIME_LSB  0x0B			// wird im RTC-Ram batteriegepuffert
#define MAX_RECORD  4095          // Maximale Anzahl an Datensätzen je EEPROM
                                  // 24LC256: 32kByte / _REC_SIZE_ - 1 = 4095

//**** Datenlogger-Funktionen
void     eeprom_speichertest  (void);
// Eingabe der aktuellen rtc-Zeit über ein Eingabeprompt
// Wartet nach Aufruf 5s auf Eingabe.
void     rs232_set_time       (void);
// Speichert oder holt den aktuellen Datensatz aus dem EEPROM
// Parameter: i2c_adress = EEPROM_1, EEPROM_2, EEPROM_3
//            record     = Datensatznummer (= Speicheradresse im EEPROM)
void     eeprom_get_record    (uint8_t i2c_address, uint16_t record);
void     eeprom_set_record    (uint8_t i2c_address, uint16_t record);
// Gibt den angegebenen Datensatz über die serielle Schnittstelle aus
// Format:   DD.MM.YY hh:mm:ss;(-)xx,y
//           Tag.Monat.Jahr Stunde:Minute:Sekunde;Temperatur,Dezimale(0,5)
//    Bsp:   02.11.17 20:47:34;21,5
void		 rs232_print_record   (uint8_t i2c_address, uint16_t record);
// Speichert oder holt die aktuelle Uhrzeit im Uhrenbaustein
void     rtc_set              (void);
void     rtc_get              (void);

// Schreibt oder liest die aktuelle Datensatznummer spannungsausfallsicher
// im Uhren-RAM (Adresse: RECORD_MSB, RECORD_LSB)
// record_number: 0x0000 ... EEPROM_END_ADDRESS (0x7fff)
uint16_t read_current_recordnumber_from_rtc (void);
void     write_current_recordnumber_to_rtc  (uint16_t record_number);
uint16_t read_current_sampletime_from_rtc (void);
void     write_current_sampletime_to_rtc  (uint16_t sample_time);
// Gibt alle gespeicherten Datensätze über RS232 aus
void     serial_print_all_records(void);
// Liest den aktuellen Temperaturwert vom LM75-Temperatursensor
int16_t lm75_read              (void);
// Zeigt den Temperaturwert auf dem LC-Display an. (z.B.: 24,5°C) 
void     lcd_print_temperatur  (int16_t degree);
// Gibt den Temperaturwert über RS232 aus. 
// mode = 0: Mit Kurvenname T1, mit \n (z.B.: T1=23,5)
// mode = 1: ohne Name, mit \r      (z.B.: 23,5) 
// mode = 2: ohne Name, mit °C mit \r  (z.B.: 23,5°c)
void     rs232_print_temperatur(int16_t degree, uint8_t mode);
uint32_t rs232_get_sampletime(void);
#endif

