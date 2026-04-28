/*
 * eeprom.h
 *
 * Created: 4/28/2026 10:29:32 AM
 *  Author: ahmed
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include <avr/io.h>
#include <avr/eeprom.h>

// EEPROM map
#define EEPROM_ZERO          0x000  // 4 bytes
#define EEPROM_SCALE         0x004  // 4 bytes
#define EEPROM_PASSWORD      0x008  // 4 bytes
#define EEPROM_WRITE_PTR     0x00C  // 2 bytes
#define EEPROM_READ_PTR      0x00E  // 2 bytes
#define EEPROM_TRANS_COUNT   0x010  // 2 bytes
#define EEPROM_FRUIT_FLAGS   0x012  // 5 bytes
#define EEPROM_FRUIT_PRICES  0x017  // 5×4 = 20 bytes
#define EEPROM_SETUP_FLAG    0x02B  // 1 byte
#define EEPROM_TRANSACTIONS  0x02C  // rest of EEPROM

#define MAX_TRANSACTIONS     78
// from hx711.c
extern long   zero_value;
extern float  scale_factor;

// from admin.c
extern char   admin_password[4];

extern uint16_t write_pointer;
extern uint16_t read_pointer;
extern uint16_t transaction_count;

void  eeprom_save_zero(long zero);
void  eeprom_save_scale(float scale);
void  eeprom_save_password(char p[4]);
void  eeprom_load_password(char p[4]);
void  eeprom_save_transaction(float weight,
float price,
uint8_t fruit);
void  eeprom_load_transaction(uint16_t index,
float *weight,
float *price,
uint8_t *fruit);
void  eeprom_clear_history(void);
void  save_defaults(void);
void  load_all_settings(void);




#endif /* EEPROM_H_ */