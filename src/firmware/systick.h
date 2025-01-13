#ifndef REMIX_FIRMWARE_SYSTICK_H_
#define REMIX_FIRMWARE_SYSTICK_H_

#include <stdint-gcc.h>

void systick_init(void);

void systick_handler(void);

void systick_delay(uint32_t ms);

uint32_t systick_time();

#endif // !REMIX_FIRMWARE_SYSTICK_H_
