/*
  Erstellt am:      1.4.2016,   22:21 Uhr
  letzte Änderung:  17.4.2016
  Datei:            SiebenSegment_mit_HCT595.c
  Autor:            Rahm
	
  Beschreibung:     Byte-I/O Test mit SiebenSegment-Ausgabe
*/

#include "controller.h"

#define HCT595 _PORTB_
#define SHIFT     0          // Shift Register Clock Pin   auf PB.0
#define STORE     1          // Storage Register Clock Pin auf PB.1
#define DATA      2          // serieller Dateneingang     auf PB.2

#define Taster _PORTD_
#define S1        1          // Taster S1 an PD.1

// const bleibt im Programmspeicher! Kein RAM-Verbrauch
const uint8_t seg7code[] = {0x7e,0x0c,0xb6,0x9e,
                            0xcc,0xda,0xfa,0x0e,
                            0xfe,0xde,0xee,0xf8,
                            0x72,0xbc,0xf2,0xe2};

// lokale Funktionsprototypen
void srg_write(uint8_t data);

// Initialisierungen
void setup( void )
{
    bit_init(HCT595,DATA ,OUT);
    bit_init(HCT595,SHIFT,OUT);
    bit_init(HCT595,STORE,OUT);
    bit_init(Taster,S1,IN);
}

int main( void )
{
    uint8_t i;
    
    setup();

    while(1)
    {
        for(i=0;i<16;i++)
        {
            srg_write(seg7code[i]);
            while(bit_read(Taster,S1) == 1);
            delay_ms(20);
            while(bit_read(Taster,S1) == 0);
            delay_ms(20);
        }
    }
}

void srg_write(uint8_t data)
{
    int8_t n;                               // n muss < 0 werden können!!

    bit_write(HCT595,STORE,0);              // STCP = 0

    //Schieberegister füllen
    bit_write(HCT595,SHIFT,0);              // SHCP = 0
    for (n = 7; n >= 0; n--)
    {
        bit_write(HCT595,DATA,data>>n);     // schieben msb first
        bit_write(HCT595,SHIFT,1);          // 0 --> 1 Flanke
        bit_write(HCT595,SHIFT,0);          // SHCP = 0
    }

    bit_write(HCT595,STORE,1);              // STCP = 1 (ausgeben)   
    bit_write(HCT595,STORE,0);              // STCP = 0
}