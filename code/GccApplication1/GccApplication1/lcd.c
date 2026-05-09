/*
 * lcd.c
 *
 * Created: 4/28/2026
 * Author: ahmed
 *
 * For 16x4 LCD (16 columns, 4 rows)
 */

#include "lcd.h"

void lcd_command(unsigned char cmd)
{
    PORTC &= ~(1 << RS);
    PORTC &= ~(1 << RW);
    LCD_DATA = cmd;
    
    PORTC |=  (1 << EN);
    _delay_us(10);
    PORTC &= ~(1 << EN);
    _delay_ms(2);
}

void lcd_data(unsigned char data)
{
    PORTC |=  (1 << RS);
    PORTC &= ~(1 << RW);
    LCD_DATA = data;
    
    PORTC |=  (1 << EN);
    _delay_us(10);
    PORTC &= ~(1 << EN);
    _delay_ms(2);
}

void lcd_init(void)
{
    LCD_DDR  = 0xFF;
    DDRC    |= (1 << RS) | (1 << RW) | (1 << EN);
    
    _delay_ms(50);
    
    lcd_command(0x38);   // 8-bit, 2 lines (4-line LCD uses 2-line mode internally)
    _delay_ms(5);
    lcd_command(0x38);
    _delay_ms(5);
    lcd_command(0x38);
    _delay_ms(5);
    
    lcd_command(0x0C);   // Display ON, cursor OFF
    _delay_ms(2);
    
    lcd_command(0x06);   // Auto-increment cursor
    _delay_ms(2);
    
    lcd_command(0x01);   // Clear display
    _delay_ms(5);
}

void lcd_clear(void)
{
    lcd_command(0x01);
    _delay_ms(5);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    // Correct addresses for 16x4 LCD
    unsigned char pos[4] = {0x80, 0xC0, 0x90, 0xD0};
    lcd_command(pos[row] + col);
}

void lcd_print(char *str)
{
    while(*str)
    {
        lcd_data(*str++);
    }
}

void lcd_print_number(long num)
{
    char buf[12];
    int i = 0;
    
    if(num == 0)
    {
        lcd_data('0');
        return;
    }
    
    if(num < 0)
    {
        lcd_data('-');
        num = -num;
    }
    
    while(num > 0)
    {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    for(int j = i - 1; j >= 0; j--)
    {
        lcd_data(buf[j]);
    }
}

void lcd_print_weight(float w)
{
    long whole   = (long)w;
    long decimal = (long)((w - whole) * 100);
    
    if(decimal < 0) decimal = -decimal;
    
    lcd_print_number(whole);
    lcd_data('.');
    
    if(decimal < 10)
    {
	    lcd_data('0');
    }
    lcd_print_number(decimal);
    lcd_print(" g");     
}	