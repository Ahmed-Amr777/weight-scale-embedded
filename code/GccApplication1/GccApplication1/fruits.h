/*
 * fruits.h
 *
 * Created: 4/28/2026 10:33:00 AM
 *  Author: ahmed
 */ 


#ifndef FRUITS_H_
#define FRUITS_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include "eeprom.h"

#define MAX_FRUITS 5

typedef struct
{
	char  name[8];
	float price_per_kg;
} Fruit;

extern Fruit fruits[MAX_FRUITS];
extern uint8_t selected_fruit;

void  fruits_init(void);
void  fruits_load_prices(void);
void  fruits_change_price(uint8_t index, float new_price);
void  fruits_reset_price(uint8_t index);
float fruits_calculate_price(float weight_grams);




#endif /* FRUITS_H_ */