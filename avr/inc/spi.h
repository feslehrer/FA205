#ifndef SPI_H_
#define SPI_H_

#define  SPI_PORT PORTB
#define  MOSI 3
#define  MISO 4
#define  SCK  5

void	spi_init( void );
uint8_t spi_transfer( uint8_t data );

#endif /* SPI_H_ */