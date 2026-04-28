/*
 * user.c
 *
 * Created: 4/28/2026 10:34:15 AM
 *  Author: ahmed
 */ 
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include "globals.h"
#include "user.h"

void check_factory_reset(void)
{
	DDRD  &= ~(1 << PD3);
	PORTD |=  (1 << PD3);
	_delay_ms(10);

	if(!(PIND & (1 << PD3)))
	{
		lcd_clear();
		lcd_goto(0, 0);
		lcd_print(" FACTORY RESET  ");
		lcd_goto(1, 0);
		lcd_print(" Remove jumper  ");
		lcd_goto(2, 0);
		lcd_print(" to continue    ");
		lcd_goto(3, 0);
		lcd_print("                ");

		eeprom_update_byte(
		(uint8_t*)EEPROM_SETUP_FLAG, 0xFF);

		while(!(PIND & (1 << PD3)));

		lcd_clear();
		lcd_goto(0, 0);
		lcd_print("  Restarting... ");
		_delay_ms(1500);

		void (*reset)(void) = 0x0000;
		reset();
	}
}

void show_startup_screen(void)
{
	lcd_clear();
	lcd_goto(0, 0);
	lcd_print("  weight Scale   ");
	lcd_goto(1, 0);
	lcd_print("  ATmega32 v1   ");
	lcd_goto(2, 0);
	lcd_print("  By: team   ");
	lcd_goto(3, 0);
	lcd_print("  Starting...   ");
	_delay_ms(3000);
}

void user_show_receipt(float weight, float price,
uint8_t fruit)
{
	lcd_clear();
	lcd_goto(0, 0);
	lcd_print("  Fruit Scale   ");
	lcd_goto(1, 0);
	lcd_print(fruits[fruit].name);
	lcd_goto(2, 0);
	lcd_print("W:");
	lcd_print_weight(weight / 1000.0);
	lcd_print(" kg");
	lcd_goto(3, 0);
	lcd_print("Total:");
	lcd_print_weight(price);
	lcd_print(" EGP");
	_delay_ms(2000);

	lcd_clear();
	lcd_goto(0, 0);
	lcd_print_number(
	(long)fruits[fruit].price_per_kg);
	lcd_print(" EGP/kg");
	lcd_goto(1, 0);
	lcd_print("Weight:");
	lcd_print_weight(weight);
	lcd_print(" g");
	lcd_goto(2, 0);
	lcd_print("Total:");
	lcd_print_weight(price);
	lcd_print(" EGP");
	lcd_goto(3, 0);
	lcd_print("  Thank you!    ");
	_delay_ms(3000);
}

void user_screen(void)
{
	static int   login_stars = 0;
	static int   counter     = 0;
	static float weight      = 0;
	static float price       = 0;

	// 1. CHECK KEYPAD FIRST (most important)
	char key = keypad_check_key();

	if(key >= '1' && key <= '5')
	{
		selected_fruit = key - '1';
		login_stars = 0;
		return;   // exit fast
	}
	else if(key == '=')
	{
		if(weight > 1.0)
		{
			eeprom_save_transaction(weight, price, selected_fruit);
			user_show_receipt(weight, price, selected_fruit);
		}
		login_stars = 0;
		return;
	}
	else if(key == 'C')
	{
		hx711_tare();
		login_stars = 0;
		return;
	}
	else if(key == '*')
	{
		login_stars++;
		if(login_stars >= 2)
		{
			login_stars = 0;
			if(admin_check_password())
			{
				current_mode = MODE_ADMIN;
				lcd_clear();
				lcd_goto(0, 0);
				lcd_print(" Welcome Admin! ");
				_delay_ms(1500);
			}
		}
		return;
	}

	// 2. UPDATE WEIGHT every 5 loops only
	counter++;
	if(counter >= 5)
	{
		counter = 0;
		weight = hx711_get_weight();
		price  = fruits_calculate_price(weight);
	}

	// 3. UPDATE DISPLAY (no clear, just overwrite)
	lcd_goto(0, 0);
	lcd_print(fruits[selected_fruit].name);
	lcd_print(" ");
	lcd_print_number((long)fruits[selected_fruit].price_per_kg);
	lcd_print("EGP ");

	lcd_goto(1, 0);
	lcd_print("W:");
	lcd_print_weight(weight / 1000.0);
	lcd_print(" kg     ");

	lcd_goto(2, 0);
	lcd_print("Total:");
	lcd_print_weight(price);
	lcd_print(" EGP    ");

	lcd_goto(3, 0);
	if(weight < 1.0)
	lcd_print("Empty 1-5 fruit ");
	else if(weight >= 5000.0)
	lcd_print("  !! OVERLOAD !!");
	else
	lcd_print("Stable = to save");
}