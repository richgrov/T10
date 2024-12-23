#ifndef REMIX_STEPPER_H_
#define REMIX_STEPPER_H_

#include "firmware/gpio.h"
#include <stdint-gcc.h>

typedef struct {
   volatile Gpio *direction_gpio;
   uint16_t steps_per_revolution;
   uint8_t timer;
   uint8_t direction_pin;
} Stepper;

void stepper_init(
   Stepper *stepper, uint16_t steps_per_revolution, uint8_t timer, volatile Gpio *direction_gpio,
   uint8_t direction_pin
);

void stepper_set_speed(Stepper *stepper, uint32_t rpm);

void stepper_set_enabled(Stepper *stepper, bool enable);

#endif // !REMIX_STEPPER_H_
