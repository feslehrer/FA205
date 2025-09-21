// Technische Richtlinie FA205
// Bibliothek:       interrupt.h
// Controller:       atmel ATMega8 / ATMega328
// Version:          2.0
// erstellt am:      13.07.2015
// letzte �nderung:  16.08.2018
// Autor:            Rahm

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "controller.h"
#include "avr/interrupt.h"

// externer Interrupt Trigger
//#define _RISING_EDGE_TRIGGER_     
#define _FALLING_EDGE_TRIGGER_
//#define _ANY_EDGE_TRIGGER_

// Funktionsprototypen f�r externen Interrupt0
extern void ext_interrupt_init    ( void (*ip) (void));
extern void ext_interrupt_enable  ( void );
extern void ext_interrupt_disable ( void );
extern void ext_interrupt_isr     ( void );

// Funktionsprototypen f�r externen Interrupt1
extern void ext_interrupt1_init    ( void (*ip) (void));
extern void ext_interrupt1_enable  ( void );
extern void ext_interrupt1_disable ( void );
extern void ext_interrupt1_isr     ( void );

// Funktionsprototypen f�r seriellen Interrupt (data received)
extern void serial_interrupt_init    ( void (*sr) (void));
extern void serial_interrupt_enable  ( void );
extern void serial_interrupt_disable ( void );
extern void serial_interrupt_isr     ( void );

// Funktionsprototypen f�r timer1ms
extern void timer1ms_init         ( void (*ti) (void) );
extern void timer1ms_enable       ( void );
extern void timer1ms_disable      ( void );
extern void timer1ms_isr          ( void );

#ifdef _ATMEGA328_
// Erweiterter Timer mit variabler Zeit: millisekunden = (16.2 ... 0.0001) ms
// Kein Bestandteil der Technischen Richtlinie FA205 !!!
// Die Funktionen werden f�r timer1ms ben�tigt. Sind aber nur beim 
// ATmega328 verf�gbar. Die Genauigkeit h�ngt vom gew�hlten Zeitwert ab!
 extern void timer_ms_init         ( void (*ti) (void), float millisec );
 extern void timer_ms_enable       ( void );
 extern void timer_ms_disable      ( void );
 extern void timer_ms_isr          ( void );
 
// Routinen zur Soundausgabe an Port B.3 (Verwendet Timer 0 = timer_ms_)
// key:       Midi-Tastennummern (49 = a' = 440Hz Kammerton) 
//                                1  = ,,A = 27,5Hz    (real sind ca. 32Hz realisierbar!)
//                         bis    88 = a''''' = 4186,01Hz
// duration:  Tondauer in ms
// silence:   Ruhe nach Ton in ms
// �nderung des Soundports hier m�glich:
 #define TON_PORT _PORTB_
 #define TON_BIT  3
 extern void sound_init(void);
 extern void play_note(uint8_t key, uint16_t duration, uint16_t silence);
 // frequenz: Tonfrequenz in Hz
 extern void note_on(float frequenz);
 extern void note_off(void);
 extern void note_isr( void );
#endif

#endif