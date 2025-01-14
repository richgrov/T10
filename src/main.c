#include <stdbool.h>
#include <stdint-gcc.h>

#include "firmware/nvic.h"
#include "firmware/rcc.h"
#include "firmware/systick.h"
#include "firmware/timer.h"
#include "firmware/usart.h"
#include "stepper.h"

void main(void) {
   systick_init();
   rcc_ahb1_enable(AHB1_ENABLE_GPIOA);
   rcc_ahb1_enable(AHB1_ENABLE_GPIOC);

   StepperController stepper = {
      .timer = 1,
      .direction_gpio = GPIOA,
      .direction_pin = 5,
      .steps_per_revolution = 200,
      .target = 60,
   };
   stepper_init(&stepper);

   usart_init(2, 115200);

   while (true) {
      stepper_update(&stepper);
   }
}

__attribute__((naked, noreturn)) void _reset(void) {
   // Zero-initialize bss
   asm volatile("  ldr r0, =_sbss\n"
                "  ldr r1, =_ebss\n"
                "  mov r2, #0\n"
                "  b begin_zero_loop\n"

                "zero_loop:\n"
                "  strb r2, [r0]\n"
                "  add r0, r0, #1\n"
                "begin_zero_loop:\n"
                "  cmp r0, r1\n"
                "  blt zero_loop\n");

   // Initialize .data
   asm volatile("  ldr r0, =_sdata\n"
                "  ldr r1, =_sidata\n"
                "  ldr r2, =_edata\n"
                "  b begin_data_init_loop\n"

                "data_init_loop:\n"
                "  ldrb r3, [r1]\n"
                "  strb r3, [r0]\n"
                "  add r0, r0, #1\n"
                "  add r1, r1, #1\n"
                "begin_data_init_loop:\n"
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
   [16 + ISR_TIM1_UP_TIM10] = stepper_tim1_update_isr,
};
