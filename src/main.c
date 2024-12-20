#include <stdbool.h>
#include <stdint-gcc.h>

#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "usart.h"

void main(void) {
   systick_init();
   rcc_ahb1_enable(AHB1_ENABLE_GPIOA);
   gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);

   const int alt_func = 7;
   gpio_set_mode(GPIOA, 2, GPIO_MODE_ALT);
   gpio_set_af(GPIOA, 2, alt_func);
   gpio_set_mode(GPIOA, 3, GPIO_MODE_ALT);
   gpio_set_af(GPIOA, 3, alt_func);

   usart_init(2, 115200);

   while (true) {
      gpio_write_pin(GPIOA, 5, true);
      usart_write(2, (uint8_t *)"LED on\r\n", 8);
      systick_delay(1000);

      gpio_write_pin(GPIOA, 5, false);
      usart_write(2, (uint8_t *)"LED off\r\n", 9);
      systick_delay(1000);
   }
}

__attribute__((naked, noreturn)) void _reset(void) {
   // Zero-initialize bss
   asm volatile("	 ldr r0, =_sbss\n"
                "	 ldr r1, =_ebss\n"
                "	 mov r2, #0\n"

                "zero_loop:\n"
                "	 strb r2, [r0]\n"
                "	 add r0, r0, #1\n"
                "	 cmp r0, r1\n"
                "  blt zero_loop\n");

   // Initialize .data
   asm volatile("  ldr r0, =_sdata\n"
                "  ldr r1, =_sidata\n"
                "  ldr r2, =_edata\n"

                "data_init_loop:\n"
                "  ldrb r3, [r1]\n"
                "  strb r3, [r0]\n"
                "  add r0, r0, #1\n"
                "  add r1, r1, #1\n"
                "  cmp r0, r2\n"
                "  blt data_init_loop\n");

   asm volatile("  bl main\n"
                "loop:\n"
                "  wfi\n"
                "  b loop\n");
}

extern void _estack(void);

__attribute__((section(".vectors"))) void (*const vector_table[16 + 97])(void) = {
   _estack,
   _reset,
   [15] = systick_handler,
};
