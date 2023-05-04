/*  
	Header:           eeprom-Funktionen
										für EEPROM 24LCxx mit i2c-interface 
  
  Autor:            Rolf Rahm
  Datum:            26.10.2017
  Letzte Änderung:  14.11.2017
*/
#ifndef _EEPROM_H
#define _EEPROM_H

#include "controller.h"

// i2c-Adressen (Definiert ist jeweils nur die Schreibadresse (R/W = 0)
#define EEPROM_1			0xa0
#define EEPROM_2			0xa2
#define EEPROM_3			0xa4
#define EEPROM_END_ADDRESS 0x7fff    // 32kByte  24LC256

//**** EEPROM-Funktionsprototypen
// Initialisierung
void     eeprom_init    (void);
// Schreibt das Byte "value" an die angegebene Speicheradresse 
// "address" = 0x0000...EEPROM_END_ADDRESS
// im angegebenen EEPROM "i2c_address" = EEPROM_1,...,EEPROM3
void     eeprom_write   (uint8_t i2c_address, uint16_t address, uint8_t value);
// dito. ein Byte lesen 
uint8_t  eeprom_read    (uint8_t i2c_address, uint16_t address);
// führt einen Speichertest des angegebenen EEPROM durch.
// getestet wird Adresse 0x0000 bis EEPROM_END_ADDRESS
// Rückgabewert: -1 = Fehler,  0 = OK
int8_t   eeprom_memtest (uint8_t i2c_address);

#endif

