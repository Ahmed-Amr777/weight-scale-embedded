/*
 * hx711.h
 *
 * Created: 4/28/2026 9:05:59 AM
 *  Author: ahmed
 */ 


#ifndef HX711_H_
#define HX711_H_

#include <avr/io.h>
#include <util/delay.h>

#define DOUT  PA0
#define SCK   PA1

void  hx711_init(void);
long  hx711_read(void);
long  hx711_average(int times);
void  hx711_tare(void);
float hx711_get_weight(void);

extern long  zero_value;
extern float scale_factor;



#endif /* HX711_H_ */