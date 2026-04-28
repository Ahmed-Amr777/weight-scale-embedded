/*
 * user.h
 *
 * Created: 4/28/2026 10:34:06 AM
 *  Author: ahmed
 */ 


#ifndef USER_H_
#define USER_H_

#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"
#include "hx711.h"
#include "fruits.h"
#include "eeprom.h"

void user_screen(void);
void user_show_receipt(float weight, float price,
uint8_t fruit);
void check_factory_reset(void);
void show_startup_screen(void);



#endif /* USER_H_ */