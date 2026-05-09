#include "hx711.h"
#include "lcd.h"
#include "globals.h"

long  zero_value   = 0;
float scale_factor = 360.065;

void hx711_init(void)
{
	// Using bitwise operations ensures the LCD pins on PORTC are NOT overwritten
	DDRC  &= ~(1 << DOUT);  // Set DOUT as input
	DDRC  |=  (1 << SCK);   // Set SCK as output
	PORTC &= ~(1 << SCK);   // Initialize SCK to LOW
}

long hx711_read(void)
{
	long result = 0;

	// Wait until DOUT goes low (data ready)
	while(PINC & (1 << DOUT));

	for(int i = 0; i < 24; i++)
	{
		PORTC |=  (1 << SCK); // SCK HIGH
		_delay_us(1);
		result <<= 1;
		
		if(PINC & (1 << DOUT))
		{
			result++;
		}
		
		PORTC &= ~(1 << SCK); // SCK LOW
		_delay_us(1);
	}

	// 25th pulse to set channel A at 128 gain
	PORTC |=  (1 << SCK);
	_delay_us(1);
	PORTC &= ~(1 << SCK);

	// Sign extension for 24-bit negative numbers
	if(result & 0x800000)
	{
		result |= 0xFF000000;
	}

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