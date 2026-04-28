/*
 * fruits.c
 *
 * Created: 4/28/2026 10:33:26 AM
 *  Author: ahmed
 */ 
#include "fruits.h"
#include "globals.h"
Fruit fruits[MAX_FRUITS] = {
{"Apple   ", 25.00},
{"Banana  ", 15.00},
{"Orange  ", 20.00},
{"Grapes  ", 45.00},
{"Mango   ", 35.00}
};

uint8_t selected_fruit = 0;

void fruits_init(void)
{
	fruits_load_prices();
}

void fruits_load_prices(void)
{
	for(int i = 0; i < MAX_FRUITS; i++)
	{
		uint8_t flag = eeprom_read_byte(
		(uint8_t*)(EEPROM_FRUIT_FLAGS + i));

		if(flag == 0x01)
		{
			uint32_t temp = eeprom_read_dword(
			(uint32_t*)(EEPROM_FRUIT_PRICES + (i*4)));
			fruits[i].price_per_kg = *(float*)&temp;
		}
	}
}

void fruits_change_price(uint8_t index, float new_price)
{
	fruits[index].price_per_kg = new_price;

	eeprom_update_dword(
	(uint32_t*)(EEPROM_FRUIT_PRICES + (index * 4)),
	*(uint32_t*)&new_price);

	eeprom_update_byte(
	(uint8_t*)(EEPROM_FRUIT_FLAGS + index), 0x01);
}

void fruits_reset_price(uint8_t index)
{
	float default_prices[5] = {25.00, 15.00,
20.00, 45.00, 35.00};

fruits[index].price_per_kg = default_prices[index];

eeprom_update_byte(
(uint8_t*)(EEPROM_FRUIT_FLAGS + index), 0x00);
}

float fruits_calculate_price(float weight_grams)
{
    return (weight_grams / 1000.0) *
            fruits[selected_fruit].price_per_kg;
}