#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "lcd.h"
#include "hx711.h"
#include "keypad.h"
#include "eeprom.h"
#include "fruits.h"
#include "admin.h"
#include "user.h"
#include "globals.h"
#define MODE_USER   0
#define MODE_ADMIN  1

unsigned char current_mode = MODE_USER;

int main(void)
{
	// 1. init LCD
	lcd_init();
	_delay_ms(50);

	// 2. check factory reset jumper
	check_factory_reset();

	// 3. check setup flag
	if(eeprom_read_byte((uint8_t*)EEPROM_SETUP_FLAG) != 0xAA)
	{
		save_defaults();
		eeprom_update_byte((uint8_t*)EEPROM_SETUP_FLAG, 0xAA);
	}

	// 4. load all settings
	load_all_settings();

	// 5. init modules
	hx711_init();
	keypad_init();

	// 6. startup screen
	show_startup_screen();

	// 7. auto zero at startup
	hx711_tare();

	// 8. main loop
	while(1)
	{
		if(current_mode == MODE_USER)
		user_screen();
		else
		admin_screen();
	}
}