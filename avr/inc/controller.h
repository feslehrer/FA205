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

#endif