#include <rc522.h>
#include <spi.h>

// Bibliotheken für RFid-Reader/Writer
// MFRC522

void rc522_write( uint8_t addr, uint8_t val)
{
	//SELECT_PORT &= ~(1<<SELECT_PIN);
	CS_High(RC522_CS);

	spi_transfer( (addr<<1) & 0x7E );
	spi_transfer( val );

	//SELECT_PORT |= (1<<SELECT_PIN);
	CS_Low(RC522_CS);
}

uint8_t rc522_read( uint8_t addr )
{
	CS_Low(RC522_CS);
	
	spi_transfer( ( ( addr<<1 ) & 0x7E ) | 0x80 );	
	uint8_t ret = spi_transfer( 0x00 );

	CS_High(RC522_CS);

	return ret;
}

void rc522_init()
{
	spi_init(RC522_CS);	
	rc522_reset();
}

void rc522_reset()
{
	rc522_write(CommandReg, PCD_RESETPHASE);
}