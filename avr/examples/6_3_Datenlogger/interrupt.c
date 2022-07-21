// Technische Richtlinie FA205
// Bibliothek:       interrupt.c
// Controller:       Atmel ATmega8 / ATmega328p
//     Verwendet externen Interrupt 0 und Timer 0
// Version:          2.0
// erstellt am:      13.07.2015
// letzte Änderung:  16.8.2018
// Autor:            Rahm

#include "interrupt.h"
#ifdef _ATMEGA8_
 #define _INT_CONFIG_     MCUCR
 #define _INT_ENABLE_     GICR
 #define _TIMER_RUN_      TCCR0
 #define _TIMER_ENABLE_   TIMSK
 #define _TIMER_INT_FLAG_ TOIE0
 #define _TIMER_CNT_      TCNT0
#endif
#ifdef _ATMEGA328_
 #define _INT_CONFIG_     EICRA
 #define _INT_ENABLE_     EIMSK
 #define _TIMER_RUN_      TCCR0B
 #define _TIMER_ENABLE_   TIMSK0
 #define _TIMER_INT_FLAG_ OCIE0A
 #define _TIMER_CNT_      TCNT0
 // Lokale initialisierungsroutine
 uint8_t preload_calc    ( float time);

 static uint16_t prescaler;
#endif

// Pointer auf Interrupt-Service-Routinen
void (*my_interrupt0) (void);
void (*my_interrupt1) (void);
void (*my_datareceived)  (void);
void (*my_timer)      (void);

//externer Interrupt 0
void ext_interrupt_init( void (*ip) (void) )
{
  my_interrupt0 = ip;
	#ifdef _FALLING_EDGE_TRIGGER_
   _INT_CONFIG_ |=  (1<<ISC01);
   _INT_CONFIG_ &= ~(1<<ISC00);    //auf fallende Flanke triggern
	#else
	 #ifdef _RISING_EDGE_TRIGGER_
	  _INT_CONFIG_ |= (1<<ISC01);
    _INT_CONFIG_ |= (1<<ISC00);   //auf steigende Flanke triggern
   #else 
	  _INT_CONFIG_ &= ~(1<<ISC01);
    _INT_CONFIG_ |=  (1<<ISC00);   //auf beliebige Flanke triggern
	 #endif
	#endif
}

ISR(INT0_vect)                    // Interrupt-Vektor
{
  _INT_ENABLE_ &= ~(1<<INT0);     // ext. Interrupt deaktivieren
  my_interrupt0();                 // Aufruf der Benutzer-ISR
  _INT_ENABLE_ |= (1<<INT0);      // ext. Interrupt aktivieren
}

void ext_interrupt_enable( void )
{
  _INT_ENABLE_ |= (1<<INT0);      // ext. Interrupt aktivieren
  sei();                          // Globale Interruptfreigabe
}

void ext_interrupt_disable( void )
{
  _INT_ENABLE_ &= ~(1<<INT0);     // ext. Interrupt deaktivieren
  cli();                          // Globale Interruptsperre
}

// Externer Interrupt 1
void ext_interrupt1_init( void (*ip) (void) )
{
	my_interrupt1 = ip;
	#ifdef _FALLING_EDGE_TRIGGER_
	 _INT_CONFIG_ |=  (1<<ISC11);
	 _INT_CONFIG_ &= ~(1<<ISC10);    //auf fallende Flanke triggern
	#else
	 #ifdef _RISING_EDGE_TRIGGER_
	  _INT_CONFIG_ |= (1<<ISC11);
	  _INT_CONFIG_ |= (1<<ISC10);   //auf steigende Flanke triggern
	 #else
	  _INT_CONFIG_ &= ~(1<<ISC11);
	  _INT_CONFIG_ |=  (1<<ISC10);   //auf beliebige Flanke triggern
	 #endif
	#endif
}
ISR(INT1_vect)                    // Interrupt-Vektor
{
	_INT_ENABLE_ &= ~(1<<INT1);     // ext. Interrupt deaktivieren
	my_interrupt1();                 // Aufruf der Benutzer-ISR
	_INT_ENABLE_ |= (1<<INT1);      // ext. Interrupt aktivieren
}

void ext_interrupt1_enable( void )
{
	_INT_ENABLE_ |= (1<<INT1);      // ext. Interrupt aktivieren
	sei();                          // Globale Interruptfreigabe
}

void ext_interrupt1_disable( void )
{
	_INT_ENABLE_ &= ~(1<<INT1);     // ext. Interrupt deaktivieren
	//cli();                          // Globale Interruptsperre
}

//*******************************************************************
//** ab hier timer-Funktionen
//*******************************************************************
// Preload = 256 - 1 ms * 3686,4 kHz / 64

void timer1ms_init ( void (*ti) (void))
{
  #ifdef _ATMEGA8_
   my_timer = ti;					// Pointer auf isr im User-Code (normalerweise: timer1ms_isr) !!
   #define PRELOAD (256 - (0.001*F_CPU / 64))		// Preloadwert
   _TIMER_CNT_ = PRELOAD;
  #endif
  
  #ifdef _ATMEGA328_
    timer_ms_init(ti,1.0);
  #endif
}

void timer1ms_enable( void )
{  
  #ifdef _ATMEGA8_
   _TIMER_CNT_ = PRELOAD;
   _TIMER_RUN_    |= (1<<CS01) | (1 << CS00);   // Timer starten mit Vorteiler 64
   _TIMER_ENABLE_ |= (1 << _TIMER_INT_FLAG_);   // Timer Interrupt aktivieren
   sei();                                       // Globale Interruptfreigabe
  #endif

  #ifdef _ATMEGA328_
    timer_ms_enable();
  #endif
}

void timer1ms_disable( void )
{
  timer_ms_disable();
}

// Die timer-Interrupt-Serviceroutine timer1ms_isr() muss
// selbst geschrieben werden!!

ISR(TIMER0_COMPA_vect)        // Timer0-Interrupt im Autoreload-mode (CTC)
{
  _TIMER_ENABLE_ &= ~(1<<_TIMER_INT_FLAG_); // Timer Interrupt sperren
  my_timer();                    // aufrufen der Interrupt-Serviceroutine
  _TIMER_ENABLE_ |= (1<<_TIMER_INT_FLAG_);  // Timer Interrupt aktivieren
}

//ISR(TIMER0_OVF_vect)        // Timer0-Interrupt im Normal-mode
//{
  //_TIMER_CNT_ = PRELOAD;         // Preloadwert
  //_TIMER_ENABLE_ &= ~(1<<_TIMER_INT_FLAG_); // Timer Interrupt sperren
  //my_timer();                    // aufrufen der Interrupt-Serviceroutine
  //_TIMER_ENABLE_ |= (1<<_TIMER_INT_FLAG_);  // Timer Interrupt aktivieren
//}

#ifdef _ATMEGA328_

void timer_ms_init ( void (*ti) (void), float time)
{
  my_timer = ti;					   // Pointer auf isr im User-Code (normalerweise: timer_ms_isr) !!
   
  TCCR0A |= (1 << WGM01);          // Timer Mode: CTC   (Autoreload-Modus)
  OCR0A = preload_calc(time);      // Bei welchem Zählwert soll der Interrupt kommen
}

uint8_t preload_calc ( float time)
{ // time in ms
  uint8_t preload;
  
  if      (time < 0.015937) prescaler = 1;         // tmax = 15,937µs
  else if (time < 0.1275  ) prescaler = 8;
  else if (time < 1.02    ) prescaler = 64;
  else if (time < 4.08    ) prescaler = 256;
  else                      prescaler = 1024;      // tmax = 16,32ms
  
  preload = (uint8_t) ((float)(F_CPU/1000/prescaler) * time - 1.0);

  return preload;
}

void timer_ms_enable( void )
{
  switch (prescaler)
  {
    case 1:     _TIMER_RUN_    |= (1 << CS00); break;               // Timer starten mit Vorteiler 1
    case 8:     _TIMER_RUN_    |= (1 << CS01); break;               // Timer starten mit Vorteiler 8
    case 64:    _TIMER_RUN_    |= (1 << CS01) | (1 << CS00); break; // Timer starten mit Vorteiler 64
    case 256:   _TIMER_RUN_    |= (1 << CS02); break;               // Timer starten mit Vorteiler 256
    case 1024:  _TIMER_RUN_    |= (1 << CS02) | (1 << CS00); break; // Timer starten mit Vorteiler 1024    
  }
  _TIMER_ENABLE_ |= (1 << _TIMER_INT_FLAG_);   // Timer Interrupt aktivieren
  sei();                                       // Globale Interruptfreigabe
}

void timer_ms_disable( void )
{
  _TIMER_RUN_ = 0x00;						// Timer stoppen
  _TIMER_ENABLE_ &= ~(1<<_TIMER_INT_FLAG_);  // Timer Interrupt sperren
  //cli();                                     // Globale Interruptsperre
}

#endif

void serial_interrupt_init( void (*sr) (void))
{ 
	rs232_init();  // Serielle Schnittstelle initialisieren
	my_datareceived = sr;
}

ISR(USART_RX_vect)                  // Interrupt-Vektor
{
	//UCSR0B &= ~(1<<RXCIE0);         // Empfangsinterrupt ausschalten
	my_datareceived();                // Aufruf der Benutzer-ISR
  //UCSR0B |= (1<<RXCIE0);          // Empfangsinterrupt einschalten
}

extern void serial_interrupt_enable  ( void )
{
  UCSR0B |= (1<<RXCIE0);            // Empfangsinterrupt einschalten
	sei();
}

extern void serial_interrupt_disable ( void )
{
	UCSR0B &= ~(1<<RXCIE0);           // Empfangsinterrupt ausschalten
}

//  
// Soundausgabe auf Lautsprecher an Port B.3
// 
void note_isr( void )
{
  bit_write(TON_PORT,TON_BIT,~bit_read(TON_PORT,TON_BIT));
}

void note_on(float frequenz)
{
  float millisec;
  
  millisec = 500/frequenz;    // Zeit für Halbe Periodendauer in ms
  
  timer_ms_init(note_isr, millisec);
  timer_ms_enable();
}

void note_off(void)
{
  timer_ms_disable();
}

void play_note(uint8_t notenwert, uint16_t duration, uint16_t silence)
{
  float exponent = ((float)notenwert - 69.0) / 12.0;
  float frequenz = 440 * pow(2,exponent);

  note_on(frequenz);
  delay_ms(duration);
  note_off();
  delay_ms(silence);
}