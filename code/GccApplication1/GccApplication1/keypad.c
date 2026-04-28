/*
 * keypad.c
 *
 * Created: 4/28/2026 9:10:32 AM
 *  Author: ahmed
 */ 
#include "keypad.h"
#include "globals.h"
unsigned char keypad_matrix[4][4] = {
{'7','8','9','/'},
{'4','5','6','*'},
{'1','2','3','-'},
{'C','0','=','+'}
};

void keypad_init(void)
{
	KEYPAD_DIR  = 0x00;
	KEYPAD_PORT = 0x0F;
}

char keypad_check_key(void)
{
	for(int col = 0; col < 4; col++)
	{
		KEYPAD_DIR  =  (0x10 << col);
		KEYPAD_PORT = ~(0x10 << col) | 0x0F;

		_delay_us(10);

		for(int row = 0; row < 4; row++)
		{
			if(!(KEYPAD_PIN & (1 << row)))
			{
				_delay_ms(20);
				if(!(KEYPAD_PIN & (1 << row)))
				{
					while(!(KEYPAD_PIN & (1 << row)));
					_delay_ms(20);
					return keypad_matrix[row][col];
				}
			}
		}
	}
	return 0;
}

char keypad_get_key(void)
{
	char key = 0;
	while(key == 0)
	{
		key = keypad_check_key();
	}
	return key;
}