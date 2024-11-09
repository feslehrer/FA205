// Technische Richtlinie FA205
// Bibliothek:       in_out.h
// Controller:       atmel ATMega8
// Version:          1.1
// erstellt am:      30.09.2015
// letzte Änderung:  09.11.2024
//                   Hinzugefügt pwm3
// Autor:            Rahm

#ifndef _IN_OUT_H_
#define _IN_OUT_H_

#include "controller.h"

//*******************************************************************************
// Globale Schalter und Symbole
// ... für Bit- und Byte-I/O
//#define NOPULLUPS
#define IN  0
#define OUT 1
// Portregisterdefinitionen. Können an dieser Stelle angepasst werden. 
#ifdef _ATtiny104_
#define _PORTA_      &PORTA			// Vorsicht! PD0 = RxD (Input) PD1 = TxD (Output) müssen beim Programmieren floaten
#endif
#ifdef _ATMEGA328_
 #define _PORTD_     &PORTD			// Vorsicht! PD0 = RxD (Input) PD1 = TxD (Output) müssen beim Programmieren floaten
 // Spezielle Portdefinitionen für Arduino-Carrier_Board
 #define LED0        _PORTD_,0
 #define LED1        _PORTD_,1
 #define LED2        _PORTD_,2
 #define LED3        _PORTD_,3
 #define LED4        _PORTD_,4
 #define LED5        _PORTD_,5
 #define LED6        _PORTD_,6
 #define LED7        _PORTD_,7
 #define SEG_a       _PORTD_,0
 #define SEG_b       _PORTD_,1
 #define SEG_c       _PORTD_,2
 #define SEG_d       _PORTD_,3
 #define SEG_e       _PORTD_,4
 #define SEG_f       _PORTD_,5
 #define SEG_g       _PORTD_,6
 #define SEG_dp      _PORTD_,7
#endif
#define _PORTB_      &PORTB			// Arduino-Pins: 8 ... 13
#define S1           _PORTB_,2
#define S2           _PORTB_,3
#define S3           _PORTB_,4
#define S4           _PORTB_,5
#define SPEAKER      _PORTB_,3
#define BACKLIGHT    _PORTB_,3

#define _PORTC_      &PORTC			// PC6 => Arduino Reset-Pin

// ... für PWM
// Der PWM-Ausgang ist standardmäßig PORTB.3 (PWM mit Timer2)
// PWM2: PD.3
// PWM3: PB.1
 
// Wenn in controller.h der Schalter auf _ATMEGA328_ steht und PWM_EXTENSION (oben) aktiviert ist,
// wird zusätzlich der PORTD.3 als zweiter PWM-Ausgang verwendet.

// ... für ADC
// Mögliche ADC-Ausgänge: PORTC  .0 | .1 | .2 | .3 | .4 | .5
//                              0x00 0x01 0x02 0x03 0x04 0x05
#define CH1 0x03	 // Kanal 1: PORTC.3
#define CH2 0x00   // Kanal 2: PORTC.0
#define CH3 0x01   // Kanal 3: PORTC.1
#define CH4 0x02   // Kanal 4: PORTC.2
#define CH5 0x04   // Kanal 5: PORTC.4 
#define CH6 0x05   // Kanal 6: PORTC.5
//*******************************************************************************

// Funktionsprototypen für Bit- und Byte Ein-/Ausgabe

void    bit_init   ( volatile uint8_t *port, uint8_t bitnr, uint8_t direction );

uint8_t bit_read   ( volatile uint8_t *port, uint8_t bitnr );

void    bit_write  ( volatile uint8_t *port, uint8_t bitnr, uint8_t value );


void    byte_init  ( volatile uint8_t *port, uint8_t direction );

uint8_t byte_read  ( volatile uint8_t *port );

void    byte_write ( volatile uint8_t *port, uint8_t value );

void    bit_toggle ( volatile uint8_t *port, uint8_t bitnr, volatile uint8_t *status );    // Keine Richtlinienfunktion

// Funktionsprototypen für PWM
void    pwm_init   ( void );
void    pwm_start  ( void );
void    pwm_stop   ( void );
void    pwm_duty_cycle ( uint8_t value);

void    pwm2_init  ( void );
void    pwm2_start ( void );
void    pwm2_stop  ( void );
void    pwm2_duty_cycle ( uint8_t value);

void    pwm3_init  ( void );
void    pwm3_start ( void );
void    pwm3_stop  ( void );
void    pwm3_duty_cycle ( uint8_t value);

// Funktionsprototypen für AD-Wandler
void    adc_init   ( void );
uint8_t adc_in1    ( void );
uint8_t adc_in2    ( void );

uint8_t adc_in     ( uint8_t channel );
#endif
