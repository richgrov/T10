#include <stdbool.h>
#include <stdint-gcc.h>

#include "gpio.h"
#include "rcc.h"
#include "systick.h"

void main(void) {
   systick_init();
   rcc_ahb1_set(AHB1_ENABLE_GPIOA);
   gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);

   while (true) {
      gpio_write_pin(GPIOA, 5, true);
      systick_delay(1000);
      gpio_write_pin(GPIOA, 5, false);
      systick_delay(1000);
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

__attribute__((section(".vectors"))) void (*const vector_table[16 + 97])(void) = {
   _estack,
   _reset,
   [15] = systick_handler,
};
