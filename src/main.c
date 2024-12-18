#include "config.h"
#include <stdbool.h>
#include <stdint-gcc.h>

typedef enum {
   AHB1_CLOCK_ENABLE_GPIOA = (1UL << 0),
} Ahb1ClockEnable;

typedef enum {
   APB2_SYSCFG_CLOCK_ENABLE = (1 << 14),
} Apb2en;

typedef struct {
   volatile uint32_t clock_ctl;
   volatile uint32_t config;
   volatile uint32_t clock_config;
   volatile uint32_t clock_interrupt;
   volatile uint32_t ahb1_reset;
   volatile uint32_t ahb2_reset;
   volatile uint32_t ahb3_reset;
   volatile uint32_t _reserved1;
   volatile uint32_t apb1_reset;
   volatile uint32_t apb2_reset;
   volatile uint32_t _reserved2;
   volatile uint32_t _reserved3;
   volatile uint32_t ahb1_clock_enable;
   volatile uint32_t ahb2_clock_enable;
   volatile uint32_t ahb3_clock_enable;
   volatile uint32_t _reserved4;
   volatile uint32_t apb1en;
   volatile uint32_t apb2en;
} ResetAndClockControl;

#define RCC ((volatile ResetAndClockControl *)0x40023800)

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

static void systick_init() {
   SysTick->control_status =
      SYSTICK_COUNTER_ENABLE | SYSTICK_INTERRUPT_ON_ZERO | SYSTICK_USE_PROCESSOR_CLOCK;
   SysTick->reload_val = CLOCK_SPEED_1MS - 1;
   SysTick->current_val = 0;
   RCC->apb2en |= APB2_SYSCFG_CLOCK_ENABLE;
}

typedef struct {
   volatile uint32_t mode;
   volatile uint32_t output_type;
   volatile uint32_t output_speed;
   volatile uint32_t pull_up_down;
   volatile uint32_t input_data;
   volatile uint32_t output_data;
   volatile uint32_t bit_set_reset;
   volatile uint32_t config_lock;
   volatile uint32_t alt_func_low;
   volatile uint32_t alt_func_high;
} Gpio;

#define GPIOA ((volatile Gpio *)0x40020000)

typedef enum {
   GPIO_MODE_INPUT,
   GPIO_MODE_OUTPUT,
   GPIO_MODE_ALT,
   GPIO_MODE_ANALOG,
} GpioPortMode;

void gpio_set_mode(volatile Gpio *gpio, uint8_t pin, GpioPortMode mode) {
   gpio->mode &= ~(3 << (pin * 2));
   gpio->mode |= mode << (pin * 2);
}

void gpio_write_pin(volatile Gpio *gpio, uint8_t pin, bool high) {
   gpio->bit_set_reset = 1 << pin << (high ? 0 : 16);
}

void spin(uint32_t delay) {
   while (delay--) {
      asm("nop");
   }
}

void main() {
   systick_init();
   RCC->ahb1_clock_enable |= AHB1_CLOCK_ENABLE_GPIOA;

   gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);

   while (true) {
      gpio_write_pin(GPIOA, 5, true);
      spin(999999);
      gpio_write_pin(GPIOA, 5, false);
      spin(999999);
   }
}

__attribute__((naked, noreturn)) void _reset() {
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

void systick_handler() {
}

__attribute__((section(".vectors"))) void (*const vector_table[16 + 97])(void) = {
   _estack,
   _reset,
   [15] = systick_handler,
};
