// Technische Richtlinie FA205
// Bibliothek:       delay.h
// Controller:       atmel ATMega8
// Version:          1.0
// erstellt am:      30.9.2015
// letzte �nderung:  9.4.2018
// Autor:            Rahm

#ifndef _DELAY_H_
#define _DELAY_H_

#include "controller.h"

// Funktionsdefinitionen f�r Technische Richtlinie FA205
extern void delay_100us ( uint16_t mikrosec);
extern void delay_ms    ( uint16_t millisec);

// erweiterte Funktionen
extern void delay_us( uint16_t mikrosekunden);
extern void delay_s( uint16_t sekunden);

#endif