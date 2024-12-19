#ifndef REMIX_SYSTICK_H_
#define REMIX_SYSTICK_H_

#include <stdint-gcc.h>

void systick_init(void);

void systick_handler(void);

void systick_delay(uint32_t ms);

#endif // !REMIX_SYSTICK_H_
