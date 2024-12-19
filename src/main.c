#include <stdbool.h>
#include <stdint-gcc.h>

#include "config.h"
#include "gpio.h"
#include "rcc.h"

static volatile uint32_t systick;

typedef enum {
   SYSTICK_COUNTER_ENABLE = 1,
   SYSTICK_INTERRUPT_ON_ZERO = (1 << 1),
   SYSTICK_USE_PROCESSOR_CLOCK = (1 << 2),
} SysTickControlStatus;

typedef struct {
   volatile uint32_t control_status;
   volatile uint32_t reload_val;
   volatile uint32_t current_val;
   volatile uint32_t calibration_val;
} SystemTick;

#define SysTick ((volatile SystemTick *)0xE000E010)

static void systick_init(void) {
   SysTick->control_status =
      SYSTICK_COUNTER_ENABLE | SYSTICK_INTERRUPT_ON_ZERO | SYSTICK_USE_PROCESSOR_CLOCK;
   SysTick->reload_val = CLOCK_SPEED_1MS - 1;
   SysTick->current_val = 0;
   rcc_apb2_set(APB2_SYSCFG_ENABLE);
}

void delay(uint32_t ticks) {
   uint32_t until = systick + ticks;
   while (systick < until) {
      asm("nop");
   }
}

void main(void) {
   systick_init();
   rcc_ahb1_set(AHB1_ENABLE_GPIOA);
   gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);

   while (true) {
      gpio_write_pin(GPIOA, 5, true);
      delay(500);
      gpio_write_pin(GPIOA, 5, false);
      delay(500);
   }
}

__attribute__((naked, noreturn)) void _reset(void) {
   extern long _sbss, _ebss, _sdata, _edata, _sidata;
   for (long *cursor = &_sbss; cursor < &_ebss; ++cursor) {
      *cursor = 0;
   }

   for (long *memory = &_sdata, *flash_data = &_sidata; memory < &_edata;) {
      *memory++ = *flash_data++;
   }

   main();

   while (true) {
   }
}

extern void _estack(void);

void systick_handler(void) {
   ++systick;
}

__attribute__((section(".vectors"))) void (*const vector_table[16 + 97])(void) = {
   _estack,
   _reset,
   [15] = systick_handler,
};
