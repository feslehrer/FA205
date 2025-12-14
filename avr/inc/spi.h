#ifndef SPI_H_
#define SPI_H_

//#define  SPI_PORT PORTB
#define  MOSI _PORTB_,3
#define  MISO _PORTB_,4
#define  SCK  _PORTB_,5
#define  CS   _PORTB_,2

void    CS_Low( uint8_t *cs_port, uint8_t cs_bit );
void    CS_High( uint8_t *cs_port, uint8_t cs_bit );

void	spi_init( uint8_t *cs_port, uint8_t cs_bit );
uint8_t spi_transfer( uint8_t data );

#endif /* SPI_H_ */