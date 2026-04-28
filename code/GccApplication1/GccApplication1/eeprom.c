/*
 * eeprom.c
 *
 * Created: 4/28/2026 10:28:58 AM
 *  Author: ahmed
 */ 
#include "eeprom.h"
#include "globals.h"
uint16_t write_pointer     = 0;
uint16_t read_pointer      = 0;
uint16_t transaction_count = 0;

void eeprom_save_zero(long zero)
{
	eeprom_update_dword((uint32_t*)EEPROM_ZERO,
	(uint32_t)zero);
}

void eeprom_save_scale(float scale)
{
	eeprom_update_dword((uint32_t*)EEPROM_SCALE,
	*(uint32_t*)&scale);
}

void eeprom_save_password(char p[4])
{
	for(int i = 0; i < 4; i++)
	eeprom_update_byte((uint8_t*)(EEPROM_PASSWORD + i),
	p[i]);
}

void eeprom_load_password(char p[4])
{
	for(int i = 0; i < 4; i++)
	p[i] = eeprom_read_byte(
	(uint8_t*)(EEPROM_PASSWORD + i));
}

void eeprom_save_transaction(float weight,
float price,
uint8_t fruit)
{
	// calculate address
	uint16_t addr = EEPROM_TRANSACTIONS +
	(write_pointer * 12);

	// save data
	eeprom_update_dword((uint32_t*)addr,
	*(uint32_t*)&weight);
	eeprom_update_dword((uint32_t*)(addr + 4),
	*(uint32_t*)&price);
	eeprom_update_byte ((uint8_t*) (addr + 8),
	fruit);

	// move write pointer
	write_pointer++;
	if(write_pointer >= MAX_TRANSACTIONS)
	write_pointer = 0;

	// if buffer full move read pointer
	if(transaction_count >= MAX_TRANSACTIONS)
	{
		read_pointer++;
		if(read_pointer >= MAX_TRANSACTIONS)
		read_pointer = 0;
	}
	else
	{
		transaction_count++;
	}

	// save pointers to EEPROM
	eeprom_update_word((uint16_t*)EEPROM_WRITE_PTR,
	write_pointer);
	eeprom_update_word((uint16_t*)EEPROM_READ_PTR,
	read_pointer);
	eeprom_update_word((uint16_t*)EEPROM_TRANS_COUNT,
	transaction_count);
}

void eeprom_load_transaction(uint16_t index,
float *weight,
float *price,
uint8_t *fruit)
{
	// calculate actual slot
	uint16_t slot = (read_pointer + index) %
	MAX_TRANSACTIONS;
	uint16_t addr = EEPROM_TRANSACTIONS + (slot * 12);

	uint32_t temp;

	temp    = eeprom_read_dword((uint32_t*)addr);
	*weight = *(float*)&temp;

	temp   = eeprom_read_dword((uint32_t*)(addr + 4));
	*price = *(float*)&temp;

	*fruit = eeprom_read_byte((uint8_t*)(addr + 8));
}

void eeprom_clear_history(void)
{
	write_pointer     = 0;
	read_pointer      = 0;
	transaction_count = 0;

	eeprom_update_word((uint16_t*)EEPROM_WRITE_PTR,  0);
	eeprom_update_word((uint16_t*)EEPROM_READ_PTR,   0);
	eeprom_update_word((uint16_t*)EEPROM_TRANS_COUNT,0);
}

void save_defaults(void)
{
	// default password 1234
char default_pass[4] = {'1','2','3','4'};
eeprom_save_password(default_pass);

// default scale factor
eeprom_save_scale(36.065);

// default zero
eeprom_save_zero(0);

// default fruit prices
float default_prices[5] = {25.00, 15.00,
                                20.00, 45.00, 35.00};
    for(int i = 0; i < 5; i++)
    {
        // clear flag = use default
        eeprom_update_byte(
            (uint8_t*)(EEPROM_FRUIT_FLAGS + i), 0x00);

        // save default price
        eeprom_update_dword(
            (uint32_t*)(EEPROM_FRUIT_PRICES + (i * 4)),
            *(uint32_t*)&default_prices[i]);
    }

    // clear history
    eeprom_clear_history();
}

void load_all_settings(void)
{
    uint32_t temp;

    // load zero
    temp       = eeprom_read_dword((uint32_t*)EEPROM_ZERO);
    zero_value = (long)temp;

    // load scale factor
    temp         = eeprom_read_dword((uint32_t*)EEPROM_SCALE);
    scale_factor = *(float*)&temp;

    // load password
    eeprom_load_password(admin_password);

    // load pointers
    write_pointer     = eeprom_read_word(
                        (uint16_t*)EEPROM_WRITE_PTR);
    read_pointer      = eeprom_read_word(
                        (uint16_t*)EEPROM_READ_PTR);
    transaction_count = eeprom_read_word(
                        (uint16_t*)EEPROM_TRANS_COUNT);

    // load fruit prices
    fruits_load_prices();
}