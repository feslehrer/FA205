// Technische Richtlinie FA205
// Bibliothek:       in_out.c
// Controller:       Atmel ATmega8/ATmega328p
// Version:          2.0
// erstellt am:      30.9.2015
// letzte Änderung:  9.4.2018
// Autor:            Rahm

#include "in_out.h"

// Definition der Funktionen
//***************************************************************
// ab hier Funktionen für Port-Ein-/Ausgabe
//***************************************************************
void bit_init(volatile uint8_t *byte_adr, uint8_t bit_nr, uint8_t direction)
{
  if (direction == OUT)
  {
    if      (byte_adr == &PORTD)  DDRD |= (0x01<<bit_nr);
    else if (byte_adr == &PORTB)  DDRB |= (0x01<<bit_nr);
    else if (byte_adr == &PORTC)  DDRC |= (0x01<<bit_nr);
  }
  else
  {
    if      (byte_adr == &PORTD)  DDRD &= ~(0x01<<bit_nr);
    else if (byte_adr == &PORTB)  DDRB &= ~(0x01<<bit_nr);
    else if (byte_adr == &PORTC)  DDRC &= ~(0x01<<bit_nr);
  }
  #ifndef	NOPULLUPS
   if (direction == IN) *byte_adr |= (0x01<<bit_nr);			// internen Pullup aktivieren
  #endif
}

void byte_init(volatile uint8_t *byte_adr, uint8_t direction)
{
  uint8_t reg_config;
	
  if (direction == OUT)
  {
    reg_config = 0xff;           // als Ausgang
  }
  else
  {
    reg_config = 0x00;           // als Eingang
  }

  if      (byte_adr == &PORTD)   DDRD = reg_config;
  else if (byte_adr == &PORTB)   DDRB = reg_config;
  else if (byte_adr == &PORTC)   DDRC = reg_config;

  #ifndef NOPULLUPS
   if (direction == IN) *byte_adr = 0xff;	// interne Pullups aktivieren
  #endif
}

uint8_t bit_read(volatile uint8_t *byte_adr, uint8_t bit_nr)
{
  if      (byte_adr == &PORTD)   return ((PIND>>bit_nr) & 0x01);
  else if (byte_adr == &PORTB)   return ((PINB>>bit_nr) & 0x01);
  else if (byte_adr == &PORTC)   return ((PINC>>bit_nr) & 0x01);
  else return 0;
}

void bit_write(volatile uint8_t *byte_adr, uint8_t bit_nr, uint8_t val)
{
  val &= 0x01;
  if (val == 1)
    *byte_adr |= (0x01<<bit_nr);
  else
    *byte_adr &= ~(0x01<<bit_nr);
}

uint8_t byte_read(volatile uint8_t *byte_adr)
{
  if      (byte_adr == &PORTD)   return PIND;
  else if (byte_adr == &PORTB)   return PINB;
  else if (byte_adr == &PORTC)   return PINC;
  else return 0;
}

//PORT als Ausgabe
void byte_write(volatile uint8_t *byte_adr, uint8_t byte_wert)
{
  *byte_adr = byte_wert;
}

void bit_toggle(volatile uint8_t *byte_adr, uint8_t bit_nr, volatile uint8_t *_bit_status_)
{
    if( !bit_read(byte_adr,bit_nr) )
    {
      delay_ms(20);                     // Entprellzeit
      while( !bit_read(byte_adr,bit_nr));
      delay_ms(20);
      *_bit_status_ = ~*_bit_status_;
      *_bit_status_ &= 0x01;                   // Bit 0 ausmaskieren!
    }
}

//***************************************************************
// ab hier PWM-Funktionen
//***************************************************************
#ifdef _ATMEGA328_
 #define _TIMER_CONT_A_       TCCR2A
 #define _TIMER_CONT_B_       TCCR2B
 #define _PWM_PRESCALER_BIT_  CS22		    // Prescaler 64  => fpwm ca. 1kHz @ 16MHz
 #define _TIMER_COMPARE_REG_2 OCR2B
 #define _OUT_MODE_BIT_2      COM2B1
 #define _TIMER_COMPARE_REG_  OCR2A
 #define _OUT_MODE_BIT_       COM2A1
#endif
#ifdef _ATMEGA8_
 #define _TIMER_CONT_A_       TCCR2
 #define _TIMER_CONT_B_       TCCR2
 #define _PWM_PRESCALER_BIT_  CS21		    // Prescaler 8  => fpwm ca. 1,6kHz @ 3,6864MHz
 #define _TIMER_COMPARE_REG_  OCR2
 #define _OUT_MODE_BIT_       COM21
#endif

void pwm_init(void)
{
  bit_init(&PORTB,3,OUT);

  _TIMER_CONT_B_ |= (1<<_PWM_PRESCALER_BIT_);   // Prescaler initialisieren
  _TIMER_CONT_A_ |= (1<<WGM21) | (1<<WGM20);    // FastPWM Mode
  _TIMER_COMPARE_REG_ = 127;
}

void pwm_start()
{
  _TIMER_CONT_A_ |= (1<<_OUT_MODE_BIT_);
}

void pwm_stop()
{
  _TIMER_CONT_A_ &= ~(1<<_OUT_MODE_BIT_);
}

void pwm_duty_cycle ( uint8_t value )
{
  _TIMER_COMPARE_REG_ = value;//255-value;
}

  void pwm2_init(void)
  {
    bit_init(&PORTD,3,OUT);
    _TIMER_CONT_B_ |= (1<<_PWM_PRESCALER_BIT_);   // Prescaler initialisieren
    _TIMER_CONT_A_ |= (1<<WGM21) | (1<<WGM20);    // FastPWM Mode
    _TIMER_COMPARE_REG_2 = 127;
  }

  void pwm2_start()
  {
    _TIMER_CONT_A_ |= (1<<_OUT_MODE_BIT_2);
  }

  void pwm2_stop()
  {
    _TIMER_CONT_A_ &= ~(1<<_OUT_MODE_BIT_2);
  }
  void pwm2_duty_cycle ( uint8_t value )
  {
    _TIMER_COMPARE_REG_2 = value;//255-value;
  }
//////// PWM3 ///////
void pwm3_init(void)
{
  bit_init(&PORTB,1,OUT);
  
  TCCR1A |= (1<<WGM10) | (1<<WGM11);                     // FastPWM Mode (8-Bit) 
  TCCR1B |= (1<<WGM12) | (1<<CS11);         // Prescaler initialisieren
  OCR1A = 127 * 4;
}

void pwm3_start()
{
    TCCR1A |= (1<<COM1A1);       // FastPWM Mode (8-Bit) 
  //_TIMER_CONT_A_ |= (1<<_OUT_MODE_BIT_2);
}

void pwm3_stop()
{
  TCCR1A &= ~(1<<COM1A1);
  //_TIMER_CONT_A_ &= ~(1<<_OUT_MODE_BIT_2);
}
void pwm3_duty_cycle ( uint8_t value )
{
  OCR1A = value*4;
  //_TIMER_COMPARE_REG_2 = value;//255-value;
}

//***************************************************************
// ab hier ADC-Funktionen
//***************************************************************
//***** Konstanten
#define AD_START    6             // Bit ADCSRA.6 startet die Wandlung

//uint16_t adc_in(uint8_t ch);       // Prototyp! Lokale Funktion mit Kanalwahl

// Initialisierung des ADU.
void adc_init(void)
{
  ADMUX  = 0x60;                  // interne Referenz / Rechtsbündig
  ADCSRA = 0x85;                  // Enable, Stop, Prescaler=32
}

// Einlesen des ADC-Kanals 1
uint8_t adc_in1( void )
{
  return (adc_in(CH1));
}

// Einlesen des ADC-Kanals 2
uint8_t adc_in2( void )
{
  return (adc_in(CH2));
}

// Funktion mit Kanal-Parameter
uint8_t adc_in(uint8_t ch)
{
  ADMUX  &=0xf0;
  ADMUX  |= ch;                     // Kanal Nr.
  ADCSRA |= (1<<AD_START);          // Wandlung starten
	
  while( ADCSRA & (1<<AD_START) );  // Warten bis Wandlung beendet

  return ADCH;
}

// Gibt den 10 Bit-Wert des ADC zurück. Funktion mit Kanal-Parameter
uint16_t adc_in10(uint8_t ch)
{
  ADMUX  &=0xf0;
  ADMUX  |= ch;                     // Kanal Nr.
  ADCSRA |= (1<<AD_START);          // Wandlung starten
  
  while( ADCSRA & (1<<AD_START) );  // Warten bis Wandlung beendet

  return (ADC>>6); //Bit nach links schieben (alternativ Flag ADLS = 1 setzen)
}