#ifndef REMIX_GPIO_H_
#define REMIX_GPIO_H_

#include <stdbool.h>
#include <stdint-gcc.h>

struct Gpio;
typedef struct Gpio Gpio;

typedef enum {
   GPIO_MODE_INPUT,
   GPIO_MODE_OUTPUT,
   GPIO_MODE_ALT,
   GPIO_MODE_ANALOG,
} GpioPortMode;

#define GPIOA ((volatile Gpio *)0x40020000)
#define GPIOB ((volatile Gpio *)0x40020400)
#define GPIOC ((volatile Gpio *)0x40020800)

void gpio_set_mode(volatile Gpio *gpio, uint8_t pin, GpioPortMode mode);
void gpio_write_pin(volatile Gpio *gpio, uint8_t pin, bool high);

#endif // !REMIX_GPIO_H_
