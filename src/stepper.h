#ifndef REMIX_STEPPER_H_
#define REMIX_STEPPER_H_

#include "firmware/gpio.h"
#include <stdint-gcc.h>

typedef struct {
   uint8_t timer;
   volatile Gpio *direction_gpio;
   uint8_t direction_pin;
   uint16_t steps_per_revolution;
   int16_t position;
   int16_t target;
   bool enabled;
} StepperController;

// Before calling this, ensure the following fields are initialized:
// timer, direction_gpio, direction_pin, steps_per_resolution
// The rest must be zero-initialized
void stepper_init(StepperController *stepper);

void stepper_update(StepperController *stepper);

void stepper_tim1_update_isr(void);

#endif // !REMIX_STEPPER_H_
