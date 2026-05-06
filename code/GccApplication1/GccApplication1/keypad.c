/*
 * keypad.c
 *
 * Created: 4/28/2026
 * Author: ahmed
 */ 
#include "keypad.h"
#include "globals.h"

// Updated to match the membrane keypad image
// Updated to match the Proteus calculator keypad exactly
unsigned char keypad_matrix[4][4] = {
{'1', '2', '3', 'A'},   // Top row
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}    // Bottom row
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

        _delay_us(20);

        for(int row = 0; row < 4; row++)
        {
            if(!(KEYPAD_PIN & (1 << row)))
            {
                _delay_ms(30);
                if(!(KEYPAD_PIN & (1 << row)))
                {
                    while(!(KEYPAD_PIN & (1 << row)));
                    _delay_ms(30);
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