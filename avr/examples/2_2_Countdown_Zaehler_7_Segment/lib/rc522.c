#include "rc522.h"

void rc522_write( uint8_t addr, uint8_t val)
{
	SELECT_PORT &= ~(1<<SELECT_PIN);
	
	spi_transfer( (addr<<1) & 0x7E );
	spi_transfer( val );
	
	SELECT_PORT |= (1<<SELECT_PIN);
}

uint8_t rc522_read( uint8_t addr )
{
	SELECT_PORT &= ~(1<<SELECT_PIN);
	
	spi_transfer( ( ( addr<<1 ) & 0x7E ) | 0x80 );
	
	uint8_t ret = spi_transfer( 0x00 );
	
	SELECT_PORT |= (1<<SELECT_PIN);
	
	return ret;
}

void rc522_init()
{
	SELECT_DDR  |= (1<<SELECT_PIN);
	SELECT_PORT |= (1<<SELECT_PIN);
	
	spi_init();
	
	rc522_reset();
}

void rc522_reset()
{
	rc522_write(CommandReg, PCD_RESETPHASE);
}