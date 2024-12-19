#include <stdbool.h>
#include <stdint-gcc.h>

#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "usart.h"

void main(void) {
   systick_init();
   rcc_ahb1_set(AHB1_ENABLE_GPIOA);
   rcc_apb1_set(APB1_UART2_ENABLE);
   gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);

   const int alt_func = 7;
   gpio_set_mode(GPIOA, 2, GPIO_MODE_ALT);
   gpio_set_af(GPIOA, 2, alt_func);
   gpio_set_mode(GPIOA, 3, GPIO_MODE_ALT);
   gpio_set_af(GPIOA, 3, alt_func);

   usart_init(USART2, 115200);

   while (true) {
      gpio_write_pin(GPIOA, 5, true);
      usart_write(USART2, (uint8_t *)"LED on\r\n", 8);
      systick_delay(1000);

      gpio_write_pin(GPIOA, 5, false);
      usart_write(USART2, (uint8_t *)"LED off\r\n", 9);
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
