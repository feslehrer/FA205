#ifndef SPI_H_
#define SPI_H_

//#define  SPI_PORT PORTB
#define  MOSI _PORTB_,3
#define  MISO _PORTB_,4
#define  SCK  _PORTB_,5
#define  CS   _PORTB_,2

void    CS_Low( void );
void    CS_High( void );

void	spi_init( void );
uint8_t spi_transfer( uint8_t data );

#endif /* SPI_H_ */