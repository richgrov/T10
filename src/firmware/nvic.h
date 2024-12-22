#ifndef REMIX_FIRMWARE_NVIC_H_
#define REMIX_FIRMWARE_NVIC_H_

#include <stdint-gcc.h>

typedef volatile uint32_t NvicEnable;

#define NVIC_ISER ((volatile NvicEnable *)0xE000E100)

#define ISR_TIM1_UP_TIM10 25

static inline void nvic_enable(uint8_t irq) {
   NVIC_ISER[irq / 32] |= (1 << (irq % 32));
}

#endif // !REMIX_FIRMWARE_NVIC_H_
