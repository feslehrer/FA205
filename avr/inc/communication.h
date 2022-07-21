// Technische Richtlinie FA205
// Bibliothek:       communication.h
// Controller:       atmel ATMega8
// Version:          1.1
// erstellt am:      5.5.2015
// letzte �nderung:  9.4.2018
// Autor:            Rahm

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "controller.h"

//*************************************************************
//Globale Definitionen
// ... f�r I2C-Funktionen
// SDA  = PC.4
// SCL  = PC.5
#define _I2C_FREQUENZ_ 40000 // 40kHz     @16MHz:   40kHz...100kHz
                             //           @3.16MHz: 8kHz ...100kHz
#define ACK  0
#define NACK 1

// ... f�r RS232-Funktionen
#define BAUD  9600 /*19200 115200*/

//*************************************************************
// Funktionsprototypen f�r I2C-Busschnittstelle (Master-Mode)
extern void    i2c_init    ( void );
extern void    i2c_start   ( void );
extern void    i2c_stop    ( void );
extern uint8_t i2c_write   ( uint8_t value);     // R�ckgabewert ACK / NACK
extern uint8_t i2c_read    ( uint8_t ack );      // R�ckgabewert Value

// Funktionsprototypen f�r RS232-Schnittstelle
extern void    rs232_init  ( void );
extern uint8_t rs232_get   ( void );
extern void    rs232_put   ( uint8_t value );
extern void    rs232_print ( uint8_t text[] );

extern void    rs232_baud    ( uint32_t baud );     // �ndert die Baudrate
extern uint8_t rs232_inputdd ( void );              // Erzeugt ein Eingabeprompt f�r eine Zahl von 00 ... 99
extern void    rs232_printdd ( uint8_t value );     // Ausgabe einer Dezimalzahl 00..99 auf RS232
extern void    rs232_int     ( uint16_t value );    // Ausgabe einer Dezimalzahl 0...65535 auf RS232
extern uint8_t rs232_binary_get ( void );
//extern uint8_t rs232_binary_get ( uint8_t *buf );   // Gibt ein empfangenes Byte im Bin�rmodus zur�ck.
                                                    // buf = empfangenes Zeichen
                                                    // R�ckgabe: 1 = Byte empfangen, 0 = keine Byte empfangen
extern uint8_t rs232_readbytes(uint8_t * buffer, int8_t length);
extern uint8_t rs232_is_received( void );           // Ist ein Byte im Empfangspuffer? Ja = 1; Nein = 0

#endif
