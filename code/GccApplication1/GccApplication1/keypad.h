/*
 * keypad.h
 *
 * Created: 4/28/2026 9:09:46 AM
 *  Author: ahmed
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>
#include <util/delay.h>

#define KEYPAD_DIR   DDRB
#define KEYPAD_PORT  PORTB
#define KEYPAD_PIN   PINB

void keypad_init(void);
char keypad_get_key(void);
char keypad_check_key(void);



#endif /* KEYPAD_H_ */