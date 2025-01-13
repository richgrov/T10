#include "systick.h"

#include <stdint-gcc.h>

#include "config.h"
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

void systick_init(void) {
   SysTick->control_status =
      SYSTICK_COUNTER_ENABLE | SYSTICK_INTERRUPT_ON_ZERO | SYSTICK_USE_PROCESSOR_CLOCK;
   SysTick->reload_val = CLOCK_SPEED / 1000 - 1;
   SysTick->current_val = 0;
   rcc_apb2_enable(APB2_SYSCFG_ENABLE);
}

void systick_handler(void) {
   ++systick;
}

void systick_delay(uint32_t ms) {
   uint32_t until = systick + ms;
   while (systick < until) {
      asm("wfi");
   }
}
