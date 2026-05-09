#ifndef HX711_H_
#define HX711_H_

#include <avr/io.h>
#include <util/delay.h>

// Updated to use PORTC pins 6 and 7
#define DOUT  PC6
#define SCK   PC7

void  hx711_init(void);
long  hx711_read(void);
long  hx711_average(int times);
void  hx711_tare(void);
float hx711_get_weight(void);

extern long  zero_value;
extern float scale_factor;

#endif /* HX711_H_ */