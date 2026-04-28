/*
 * hx711.c
 *
 * Created: 4/28/2026 9:06:10 AM
 *  Author: ahmed
 */ 
#include "hx711.h"
#include "lcd.h"
#include "globals.h"
long  zero_value   = 0;
float scale_factor = 36.065;

void hx711_init(void)
{
	DDRA &= ~(1 << DOUT);
	DDRA |=  (1 << SCK);
	PORTA &= ~(1 << SCK);
}

long hx711_read(void)
{
	long result = 0;

	while(PINA & (1 << DOUT));

	for(int i = 0; i < 24; i++)
	{
		PORTA |=  (1 << SCK);
		_delay_us(1);
		result <<= 1;
		if(PINA & (1 << DOUT))
		result++;
		PORTA &= ~(1 << SCK);
		_delay_us(1);
	}

	PORTA |=  (1 << SCK);
	_delay_us(1);
	PORTA &= ~(1 << SCK);

	if(result & 0x800000)
	result |= 0xFF000000;

	return result;
}

long hx711_average(int times)
{
	long sum = 0;
	for(int i = 0; i < times; i++)
	{
		sum += hx711_read();
		_delay_ms(10);
	}
	return sum / times;
}

void hx711_tare(void)
{
	lcd_clear();
	lcd_goto(0, 0);
	lcd_print("Please wait...  ");
	lcd_goto(1, 0);
	lcd_print("Calculating zero");
	lcd_goto(2, 0);
	lcd_print("Do not touch    ");
	lcd_goto(3, 0);
	lcd_print("the scale!      ");

	zero_value = hx711_average(20);

	lcd_clear();
	lcd_goto(0, 0);
	lcd_print("Zero set!       ");
	lcd_goto(1, 0);
	lcd_print("Place weight... ");
	_delay_ms(2000);
}

float hx711_get_weight(void)
{
	long  raw = hx711_average(3);
	float w   = (float)(raw - zero_value) / scale_factor;

	if(w < 0) w = 0;
	return w;
}