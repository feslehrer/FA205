// Technische Richtlinie FA205
// Bibliothek:       controller.h
// Controller:       atmel ATMega8/ATMega328
// Version:          1.0
// erstellt am:      30.9.2015
// letzte Änderung:  17.3.2018
// Autor:            Rahm

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// Controllerspezifische Registerdefinitionen
#include "avr/io.h"
#include "math.h"

#ifdef __AVR_ATmega8__
 #define _ATMEGA8_      // z.B. MyAVR
#endif
#ifdef __AVR_ATmega328P__
 #define _ATMEGA328_     // z.B. Arduino UNO
#endif
#ifdef __AVR_ATmega328PB__
  #define _ATMEGA328_     // z.B. Arduino UNO
  #define _ATMEGA328PB_	  // ATmega328PB_Xplained-Board 
#endif
#ifdef __AVR_ATtiny104__
  #define _ATtiny104_     // z.B. Arduino UNO
#endif

// F_CPU beeinflusst timer1ms, delay, rs232, pwm
//#define F_CPU 3686400UL     // z.B. myAVR
#define F_CPU 16000000UL	// z.B. Arduino UNO

#define __DELAY_BACKWARD_COMPATIBLE__

// Technische Richtlinie FA205 Datentyp-Definitionen
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned long uint32_t;
typedef signed   char int8_t;
typedef signed   int  int16_t;
typedef signed   long int32_t;

// Technische Richtlinie FA205 Bibliotheken
#include "communication.h"
#include "in_out.h"
#include "interrupt.h"
#include "lcd.h"
#include "delay.h"

// AVR Portpins
#define PD0 _PORTD_,0
#define PD1 _PORTD_,1
#define PD2 _PORTD_,2
#define PD3 _PORTD_,3
#define PD4 _PORTD_,4
#define PD5 _PORTD_,5
#define PD6 _PORTD_,6
#define PD7 _PORTD_,7

#define PB0 _PORTB_,0
#define PB1 _PORTB_,1
#define PB2 _PORTB_,2
#define PB3 _PORTB_,3
#define PB4 _PORTB_,4
#define PB5 _PORTB_,5
#define PB6 _PORTB_,6
#define PB7 _PORTB_,7

#define PC0 _PORTC_,0
#define PC1 _PORTC_,1
#define PC2 _PORTC_,2
#define PC3 _PORTC_,3
#define PC4 _PORTC_,4
#define PC5 _PORTC_,5
#define PC6 _PORTC_,6
#define PC7 _PORTC_,7

#endif