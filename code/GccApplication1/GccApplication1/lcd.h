/*
 * lcd.h
 *
 * Created: 4/28/2026 9:12:17 AM
 *  Author: ahmed
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA    PORTA
#define LCD_DDR     DDRA
#define RS  PC0
#define RW  PC1
#define EN  PC3

void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_print(char *str);
void lcd_print_number(long num);
void lcd_print_weight(float w);
void lcd_clear(void);
void lcd_goto(unsigned char row, unsigned char col);



#endif /* LCD_H_ */