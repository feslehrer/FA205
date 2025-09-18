/* 
  Beschreibung: 

  Autor:            Rolf Rahm
  Datum:            23.06.2016
  Letzte Änderung:  23.06.2016
*/
#include "controller.h"

// 1 RAM/CLK A4 A3 A2 A1 A0 R/W

#include "ds1302.h"


/*low-level-routine which writes byte*/
void ds1302_lowlevel_write ( uint8_t value);

/*low-level-routine which reads byte*/
uint8_t ds1302_lowlevel_read( void );

void ds1302_init(void)
{
    bit_init(RTC_PORT,RTC_CE,OUT);
    bit_init(RTC_PORT,RTC_CLK,OUT);
    ds1302_write (CONTROL, 0);    // Bit WP = 0
}

uint8_t ds1302_read (uint8_t adress)
{
    uint8_t temp;

    bit_write(RTC_PORT,RTC_CE,1);    //RTC_PORT |= (1<<RTC_CE);

    ds1302_lowlevel_write (adress+1);
    temp = ds1302_lowlevel_read();

    bit_write(RTC_PORT,RTC_CE,0);    //RTC_PORT &= ~(1<<RTC_CE);

    return temp;
}

void ds1302_write(uint8_t adress, uint8_t byte)
{   
    bit_write(RTC_PORT,RTC_CE,1);            // CE -> 1

    ds1302_lowlevel_write (adress);
    ds1302_lowlevel_write (byte);

    bit_write(RTC_PORT,RTC_CE,0);            // CE -> 0
}


void ds1302_lowlevel_write (uint8_t byte)
{
    uint8_t i;

    bit_init(RTC_PORT,RTC_IO,OUT);            // IO als Ausgang
    bit_write(RTC_PORT,RTC_CLK,0);            // CLK -> 0

    for(i = 0; i < 8; i++)
    {        
        bit_write(RTC_PORT,RTC_IO,(byte&(1<<i))!=0);

        bit_write(RTC_PORT,RTC_CLK,1);        // CLK -> 1
        _delay_us(2);
        bit_write(RTC_PORT,RTC_CLK,0);        // CLK -> 0
        _delay_us(2);
    }
}

uint8_t ds1302_lowlevel_read (void)
{
    uint8_t byte = 0;
    uint8_t i;

    bit_init(RTC_PORT,RTC_IO,IN);         // IO als Eingang

    for (i=0; i<8; i++)
    {
        if(bit_read(RTC_PORT,RTC_IO))     // IO-bit lesen
        byte |= (1<<i);

        bit_write(RTC_PORT,RTC_CLK,1);    // CLK -> 1
        _delay_us(2);
        bit_write(RTC_PORT,RTC_CLK,0);    // CLK -> 0
        _delay_us(2);
    }
    return byte;
}

void ds1302_get_time ( struct ds1302_data *dataset )
{
    uint8_t temp = 0;

    temp = ds1302_read (SEC);
    dataset->sec = (temp & 0x0f) + ((temp & 0x70)>>4)*10;
    temp = ds1302_read (MIN);
    dataset->min = (temp & 0x0f) + ((temp & 0x70)>>4)*10;
    temp = ds1302_read (HR);
    dataset->hrs = (temp & 0x0f) + ((temp & 0x30)>>4)*10;
    temp = ds1302_read (DATE);
    dataset->date = (temp & 0x0f) + ((temp & 0x30)>>4)*10;
    temp = ds1302_read (MONTH);
    dataset->month = (temp & 0x0f) + ((temp & 0x10)>>4)*10;
    temp = ds1302_read (DAY);
    dataset->day = temp & 0x07;
    temp = ds1302_read (YEAR);
    dataset->year = (temp & 0x0f) + ((temp & 0xf0)>>4)*10;
    ds1302_write (CONTROL, 0x80);
}

void ds1302_set_time (struct ds1302_data *dataset)
{   uint8_t temp;

    ds1302_write (CONTROL, 0);     // Schreibschutz aus

    temp = (dataset->sec) & 0x7f;
    ds1302_write (SEC,   ((temp/10)<<4) | (temp%10));
    temp = (dataset->min) & 0x7f;
    ds1302_write (MIN,   ((temp/10)<<4) | (temp%10));
    temp = (dataset->hrs) & 0x3f;
    ds1302_write (HR,    ((temp/10)<<4) | (temp%10));
    temp = (dataset->date) & 0x3f;
    ds1302_write (DATE,  ((temp/10)<<4) | (temp%10));
    temp = (dataset->month) & 0x1f;
    ds1302_write (MONTH, ((temp/10)<<4) | (temp%10));
    temp = (dataset->day) & 0x07;
    ds1302_write (DAY, temp);
    temp = (dataset->year) & 0xff;
    ds1302_write (YEAR,  ((temp/10)<<4) | (temp%10));

    ds1302_write (CONTROL, 0x80);   // Schreibschutz ein
}


void ds1302_get_end_time ( struct ds1302_data *dataset )
{
    //ds1302_write (CONTROL, 0);
    dataset->sec   = ds1302_read (RAM1);
    dataset->min   = ds1302_read (RAM2);
    dataset->hrs   = ds1302_read (RAM3);
    dataset->date  = ds1302_read (RAM4);
    dataset->month = ds1302_read (RAM5);
    dataset->day   = ds1302_read (RAM6);
    dataset->year  = ds1302_read (RAM7);
    //ds1302_write (CONTROL, 0x80);
}

void ds1302_set_end_time (struct ds1302_data *dataset)
{
    ds1302_write (CONTROL, 0);     // Schreibschutz aus

    ds1302_write (RAM1, dataset->sec);
    ds1302_write (RAM2, dataset->min);
    ds1302_write (RAM3, dataset->hrs);
    ds1302_write (RAM4, dataset->date);
    ds1302_write (RAM5, dataset->month);
    ds1302_write (RAM6, dataset->day);
    ds1302_write (RAM7, dataset->year);

    ds1302_write (CONTROL, 0x80);   // Schreibschutz ein
}
