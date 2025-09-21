/*
 * nunchuk.h
 * Version 2.0
 * Created: 29.03.2018
 *  Author: Rahm
 *  letzte Änderung:   10.4.18
 */ 


#ifndef NUNCHUK_H_
#define NUNCHUK_H_

// Nunchuk I²C-Adressen
#define NUNCHUK_ADDR_R 	0xA5
#define NUNCHUK_ADDR_W  0xA4

//Globale Variablen zur Übergabe der Nunchuk Sensordaten
//uint8_t nun_buf[6];
uint16_t X_accel, Y_accel, Z_accel;
uint8_t  X_joy, Y_joy;
uint8_t  Z_button, C_button;

//Nunchuk Funktionsprototypen
void nunchuk_init  ( void );
void nunchuk_read  ( void );
void nunchuk_debug ( void );

#endif /* NUNCHUK_H_ */