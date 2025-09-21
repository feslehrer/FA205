#include <avr/io.h>
#include "controller.h"
#include "spi.h"

void spi_init( void )
{
	bit_init(MISO,IN);
  bit_init(MOSI,OUT);
	bit_init(SCK,OUT);
  bit_init(CS,OUT);
	
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);		// Enable SPI, Set as Master, Prescaler: Fosc/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);		// Enable SPI, Set as Master, Prescaler: Fosc/32
}

// Chip Select steuern (z.?B. PB2 als CS)
void CS_Low(void) 
{
  bit_write(CS,0);
}

void CS_High(void) 
{
  bit_write(CS,1);
}

uint8_t spi_transfer( uint8_t data )
{
	SPDR = data;								  // Load data into the buffer
	while(!(SPSR & (1<<SPIF)));		// Wait until transmission complete
	
	return(SPDR);								  // Return received data
}
