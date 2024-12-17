#include <stdbool.h>
#include <stdint-gcc.h>

typedef enum {
   AHB1_CLOCK_ENABLE_GPIOA = (1UL << 0),
} Ahb1ClockEnable;

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
} ResetAndClockControl;

#define RCC ((volatile ResetAndClockControl *)0x40023800)

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
   GPIO_MODE_INPUT = 0b00,
   GPIO_MODE_OUTPUT = 0b01,
   GPIO_MODE_ALT = 0b10,
   GPIO_MODE_ANALOG = 0b11,
} GpioPortMode;

void gpio_set_mode(volatile Gpio *gpio, uint8_t pin, GpioPortMode mode) {
   gpio->mode &= ~(0b11UL << (pin * 2));
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

__attribute__((section(".vectors"))) void (*const vector_table[16 + 97])(void) = {
   _estack,
   _reset,
};
