#ifndef _DS1302_H
#define _DS1302_H

#include "controller.h"
#include <util/delay.h>

/*connection settings*/
#define RTC_CE		3
#define RTC_IO		2
#define RTC_CLK		1
#define RTC_PORT    _PORTB_

/*command settings*/
#define SEC				0x80
#define MIN				0x82
#define HR				0x84
#define DATE			0x86
#define MONTH			0x88
#define DAY				0x8A
#define YEAR			0x8C
#define CONTROL			0x8E
#define TRICKLECHARGE	0x90
#define CLOCKBURST		0xBE

/*RAM may be used, if neccessary*/
	#define RAM0		0xC0
	#define RAM1		0xC2
	#define RAM2		0xC6
	#define RAM3		0xC8
	#define RAM4		0xCA
	#define RAM5		0xCC
	#define RAM6		0xCE
	#define RAM7		0xD0
    #define RAM8        0xD2
    
/*	...
	...
	...
	#define RAM30		0x
	#define RAMBURST	0xFC
*/

/*global*/
struct ds1302_data
{
    int8_t sec;
    int8_t min;
    int8_t hrs;
    int8_t date;
    int8_t month;
    int8_t day;
    int8_t year;
};

void ds1302_init(void);

/*write byte in specified register*/
void ds1302_write(uint8_t command, uint8_t value);

/*read byte out of specified register*/
uint8_t ds1302_read(uint8_t command);

/*get date and time -> data ist given to struct*/
void ds1302_get_time (struct ds1302_data *dataset);
void ds1302_get_end_time ( struct ds1302_data *dataset );

/*set date and time*/
void ds1302_set_time (struct ds1302_data *dataset);
void ds1302_set_end_time (struct ds1302_data *dataset);

#endif

