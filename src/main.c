#include <stdbool.h>
#include <stdint-gcc.h>

#include "adv_ctl_timer.h"
#include "rcc.h"
#include "systick.h"
#include "usart.h"

void main(void) {
   systick_init();
   rcc_ahb1_enable(AHB1_ENABLE_GPIOA);
   rcc_ahb1_enable(AHB1_ENABLE_GPIOC);

   uint16_t brightness = 0;
   uint16_t direction = 1;
   uint8_t timer = 1;

   adv_ctl_timer_init(1);
   adv_ctl_timer_pwm_init(1);
   adv_ctl_timer_pwm_config(1, 1, 500, brightness);
   adv_ctl_timer_pwm_start(1);

   adv_ctl_timer_init(8);
   adv_ctl_timer_pwm_init(8);
   adv_ctl_timer_pwm_config(8, 1, 500, brightness);
   adv_ctl_timer_pwm_start(8);

   usart_init(2, 115200);

   while (true) {
      brightness += direction;
      if (brightness >= 500) {
         direction = -1;
      } else if (brightness == 0) {
         direction = 1;
         if (timer == 1) {
            timer = 8;
         } else if (timer == 8) {
            timer = 1;
         }
      }

      adv_ctl_timer_pwm_config(timer, 1, 500, brightness);
      systick_delay(1);
   }
}

__attribute__((naked, noreturn)) void _reset(void) {
   // Zero-initialize bss
   asm volatile("  ldr r0, =_sbss\n"
                "  ldr r1, =_ebss\n"
                "  mov r2, #0\n"

                "zero_loop:\n"
                "  strb r2, [r0]\n"
                "  add r0, r0, #1\n"
                "  cmp r0, r1\n"
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
