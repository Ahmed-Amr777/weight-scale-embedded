/*
 * admin.h
 *
 * Created: 4/28/2026 10:25:05 AM
 *  Author: ahmed
 */ 


#ifndef ADMIN_H_
#define ADMIN_H_

#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"
#include "eeprom.h"
#include "fruits.h"
#include "hx711.h"

extern char admin_password[4];

void admin_screen(void);
void  admin_view_history(void);
void  admin_edit_prices(void);
void  admin_calibrate(void);
void  admin_set_zero(void);
void  admin_change_password(void);
void  admin_clear_history(void);
unsigned char admin_check_password(void);



#endif /* ADMIN_H_ */