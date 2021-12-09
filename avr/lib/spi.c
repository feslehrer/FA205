#include <avr/io.h>
#include "controller.h"
#include "spi.h"

void spi_init( void )
{
	//DDRB |= (1<<PB2) | (1<<PB5);				// MOSI, SCK as Output	
	bit_init(SPI_PORT,MOSI,OUT);
	bit_init(SPI_PORT,MISO,IN);
	bit_init(SPI_PORT,SCK,OUT);
	
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);		// Enable SPI, Set as Master, Prescaler: Fosc/16
}

uint8_t spi_transfer( uint8_t data )
{
	SPDR = data;								// Load data into the buffer
	while(!(SPSR & (1<<SPIF)));					// Wait until transmission complete
	
	return(SPDR);								// Return received data
}