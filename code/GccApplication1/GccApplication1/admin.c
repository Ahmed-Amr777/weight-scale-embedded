/*
 * admin.c
 *
 * Created: 4/28/2026 10:24:57 AM
 *  Author: ahmed
 */ 
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "admin.h"
#include "globals.h"
char admin_password[4] = {'1','2','3','4'};

unsigned char admin_check_password(void)
{
    char input[4] = {0,0,0,0};
    int  index    = 0;

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("  ADMIN LOGIN   ");
    lcd_goto(1, 0);
    lcd_print("Enter password: ");
    lcd_goto(2, 0);
    lcd_print("____            ");
    lcd_goto(3, 0);
    lcd_print("C=Cancel        ");

    while(index < 4)
    {
        char key = keypad_get_key();

        if(key == 'C')
            return 0;   // cancelled

        if(key >= '0' && key <= '9')
        {
            input[index] = key;
            lcd_goto(2, index);
            lcd_data('*');
            index++;
        }
    }

    // check password
    for(int i = 0; i < 4; i++)
    {
        if(input[i] != admin_password[i])
        {
            lcd_clear();
            lcd_goto(0, 0);
            lcd_print(" Wrong password!");
            lcd_goto(1, 0);
            lcd_print(" Access denied  ");
            _delay_ms(2000);
            return 0;
        }
    }

    return 1;   // correct
}

void admin_screen(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("ADMIN MENU A=Exit");
    lcd_goto(1, 0);
    lcd_print("1=Hist 2=Prices");
    lcd_goto(2, 0);
    lcd_print("3=Calib 4=Zero");
    lcd_goto(3, 0);
    lcd_print("5=Pass 6=Clear");

    char key = keypad_get_key();

    switch(key)
    {
        case '1': admin_view_history();    break;
        case '2': admin_edit_prices();     break;
        case '3': admin_calibrate();       break;
        case '4': admin_set_zero();        break;
        case '5': admin_change_password(); break;
        case '6': admin_clear_history();   break;
        case 'A':
            current_mode = MODE_USER;
            lcd_clear();
            lcd_goto(0, 0);
            lcd_print(" Exiting Admin  ");
            _delay_ms(1500);
            break;
    }
}

void admin_view_history(void)
{
    if(transaction_count == 0)
    {
        lcd_clear();
        lcd_goto(0, 0);
        lcd_print(" No history yet ");
        _delay_ms(2000);
        return;
    }

    uint16_t index = 0;

    while(1)
    {
        float   weight;
        float   price;
        uint8_t fruit;

        eeprom_load_transaction(index,
                                &weight,
                                &price,
                                &fruit);

        lcd_clear();
        lcd_goto(0, 0);
        lcd_print("T:");
        lcd_print_number(index + 1);
        lcd_print("/");
        lcd_print_number(transaction_count);

        lcd_goto(1, 0);
        lcd_print(fruits[fruit].name);

        lcd_goto(2, 0);
        lcd_print("W:");
        lcd_print_weight(weight);
        lcd_print(" g  ");

        lcd_goto(3, 0);
        lcd_print("P:");
        lcd_print_weight(price);
        lcd_print(" EGP");

        char key = keypad_get_key();

        if(key == 'C' && index < transaction_count - 1)
            index++;
        else if(key == 'B' && index > 0)
            index--;
        else if(key == 'C')
            break;
    }
}

void admin_edit_prices(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Select fruit:   ");
    lcd_goto(1, 0);
    lcd_print("1=Apple 2=Banana");
    lcd_goto(2, 0);
    lcd_print("3=Orange4=Grapes");
    lcd_goto(3, 0);
    lcd_print("5=Mango A=Back  ");

    char key = keypad_get_key();

    if(key == 'A') return;

    int fruit_index = key - '1';
    if(fruit_index < 0 || fruit_index > 4) return;

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print(fruits[fruit_index].name);
    lcd_goto(1, 0);
    lcd_print("Current:");
    lcd_print_number((long)fruits[fruit_index].price_per_kg);
    lcd_print(" EGP");
    lcd_goto(2, 0);
    lcd_print("New price:      ");
    lcd_goto(3, 0);
    lcd_print("A=Cancel D=Confirm");

    // get new price digits
    char digits[4] = {'0','0','0','0'};
    int  index     = 0;

    while(1)
    {
        char k = keypad_get_key();

        if(k == 'A') return;

        if(k == 'D' && index > 0)
        {
            // calculate price from digits
            float new_price = 0;
            for(int i = 0; i < index; i++)
            {
                new_price = new_price * 10 +
                            (digits[i] - '0');
            }

            fruits_change_price(fruit_index, new_price);

            lcd_clear();
            lcd_goto(0, 0);
            lcd_print(fruits[fruit_index].name);
            lcd_goto(1, 0);
            lcd_print("New price:      ");
            lcd_goto(2, 0);
            lcd_print_number((long)new_price);
            lcd_print(" EGP/kg");
            lcd_goto(3, 0);
            lcd_print("Saved!          ");
            _delay_ms(2000);
            return;
        }

        if(k >= '0' && k <= '9' && index < 4)
        {
            digits[index++] = k;
            lcd_goto(2, 10 + index - 1);
            lcd_data(k);
        }
    }
}

void admin_calibrate(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Calibration     ");
    lcd_goto(1, 0);
    lcd_print("Place 1kg weight");
    lcd_goto(2, 0);
    lcd_print("on scale        ");
    lcd_goto(3, 0);
    lcd_print("Press D when    ");
    _delay_ms(1000);

    lcd_goto(3, 0);
    lcd_print("ready...        ");

    char key = keypad_get_key();
    if(key == 'A') return;

    // read raw with 1kg
    long raw_1kg = hx711_average(20);

    // calculate scale factor
    scale_factor = (float)(raw_1kg - zero_value) / 1000.0;

    // save to EEPROM
    eeprom_save_scale(scale_factor);

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Calibrated!     ");
    lcd_goto(1, 0);
    lcd_print("Factor:         ");
    lcd_goto(2, 0);
    lcd_print_number((long)scale_factor);
    lcd_goto(3, 0);
    lcd_print("Saved!          ");
    _delay_ms(2000);
}

void admin_set_zero(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Setting Zero    ");
    lcd_goto(1, 0);
    lcd_print("Remove all      ");
    lcd_goto(2, 0);
    lcd_print("weight from     ");
    lcd_goto(3, 0);
    lcd_print("scale press D   ");

    char key = keypad_get_key();
    if(key == 'A') return;

    hx711_tare();
    eeprom_save_zero(zero_value);

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Zero set!       ");
    lcd_goto(1, 0);
    lcd_print("Saved to memory ");
    _delay_ms(2000);
}

void admin_change_password(void)
{
    char new_pass[4] = {0,0,0,0};
    int  index       = 0;

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("New password:   ");
    lcd_goto(1, 0);
    lcd_print("Enter 4 digits  ");
    lcd_goto(2, 0);
    lcd_print("____            ");
    lcd_goto(3, 0);
    lcd_print("A=Cancel        ");

    while(index < 4)
    {
        char key = keypad_get_key();

        if(key == 'A') return;

        if(key >= '0' && key <= '9')
        {
            new_pass[index] = key;
            lcd_goto(2, index);
            lcd_data('*');
            index++;
        }
    }

    // confirm new password
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Confirm:        ");
    lcd_goto(1, 0);
    lcd_print("____            ");

    char confirm[4] = {0,0,0,0};
    index = 0;

    while(index < 4)
    {
        char key = keypad_get_key();

        if(key == 'C') return;

        if(key >= '0' && key <= '9')
        {
            confirm[index] = key;
            lcd_goto(1, index);
            lcd_data('*');
            index++;
        }
    }

    // check match
    for(int i = 0; i < 4; i++)
    {
        if(new_pass[i] != confirm[i])
        {
            lcd_clear();
            lcd_goto(0, 0);
            lcd_print("Passwords dont  ");
            lcd_goto(1, 0);
            lcd_print("match! Try again");
            _delay_ms(2000);
            return;
        }
    }

    // save new password
    for(int i = 0; i < 4; i++)
        admin_password[i] = new_pass[i];

    eeprom_save_password(admin_password);

    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Password changed");
    lcd_goto(1, 0);
    lcd_print("Saved!          ");
    _delay_ms(2000);
}

void admin_clear_history(void)
{
    lcd_clear();
    lcd_goto(0, 0);
    lcd_print("Clear history?  ");
    lcd_goto(1, 0);
    lcd_print("ALL data lost!  ");
    lcd_goto(2, 0);
    lcd_print("D = confirm       ");
    lcd_goto(3, 0);
    lcd_print("A = cancel        ");

    char key = keypad_get_key();

    if(key == 'A')
    {
        eeprom_clear_history();

        lcd_clear();
        lcd_goto(0, 0);
        lcd_print("History cleared!");
        lcd_goto(1, 0);
        lcd_print("All slots free  ");
        _delay_ms(2000);
    }
}