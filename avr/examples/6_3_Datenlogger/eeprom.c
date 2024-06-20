/* 
  Beschreibung:     Byteweises Schreiben und Lesen von EEPROM
										24LCxx am i2c-Bus

  Autor:            Rolf Rahm
  Datum:            26.10.2017
  Letzte Änderung:  18.03.2018
*/
#include "controller.h"
#include "eeprom.h"

void eeprom_init(void)
{
  i2c_init();     // Zugriff auf EEPROM über i2C-Bus
}

uint8_t eeprom_read (uint8_t i2c_address, uint16_t address)
{
  volatile uint8_t temp;
  uint8_t response;
  

  i2c_start();
  response = i2c_write(i2c_address);				// I2C-Adresse EEPROM1
  if (response == NACK) return -1;
  i2c_write((address>>8) & 0x00ff);	// Address High Byte schreiben
	i2c_write(address & 0x00ff);			// Address Low Byte schreiben
	
  i2c_start();								// Repeated Start
	i2c_write(i2c_address | 0x01);
  temp = i2c_read(NACK);
  i2c_stop();
  return temp;
}

void eeprom_write(uint8_t i2c_address, uint16_t address, uint8_t value)
{
	uint8_t	response;
	
  i2c_start();
  i2c_write(i2c_address);
  i2c_write((address>>8) & 0x00ff);				// EEPROM-Adresse High Byte schreiben
	i2c_write(address & 0x00ff);						// EEPROM-Adresse Low Byte schreiben
  
  i2c_write(value);
  i2c_stop();
	delay_100us(1);													// Kurze Pause nötig!!
	response = NACK;
	do																			// Acknowledge-Polling bis Schreibvorgang
	{																				// beendet.
		i2c_start();
		response = i2c_write(i2c_address);
	} while (response == NACK);
	i2c_stop();
}
// testet den gewählten EEPROM durch wiederholtes
// schreiben und lesen von 0xaa und 0x55 auf jede
// Speicherstelle bis zur Endadresse (eeprom.h)
// Der ursrüngliche Wert wird wieder hergestellt.
int8_t eeprom_memtest(uint8_t i2c_address)
{
	uint32_t adresse;
	uint8_t buf;
  int16_t temp;
	
	for(adresse = 0x0000;adresse <= EEPROM_END_ADDRESS;adresse++)
	{
    lcd_setcursor(2,1);
    lcd_int((uint16_t)adresse);
		
    temp = eeprom_read(i2c_address,(uint16_t)adresse); if (temp == -1) return -1;
		eeprom_write(i2c_address,(uint16_t)adresse,0xAA);
		buf = (uint8_t) eeprom_read(i2c_address,(uint16_t)adresse);	if (buf != 0xAA)	{return -1;}
		eeprom_write(i2c_address,(uint16_t)adresse,0x55);
		buf = (uint8_t) eeprom_read(i2c_address,(uint16_t)adresse);	if (buf != 0x55)  {return -1;}
		eeprom_write(i2c_address,(uint16_t)adresse,(uint8_t)temp);
	}

  return 0;
}
