#include "stepper.h"

#include "config.h"
#include "firmware/adv_ctl_timer.h"

#define CYCLE_WIDTH 50
#define DUTY_CYCLE 25

void stepper_init(
   Stepper *stepper, uint16_t steps_per_revolution, uint8_t timer, volatile Gpio *direction_gpio,
   uint8_t direction_pin
) {
   stepper->timer = timer;
   stepper->direction_gpio = direction_gpio;
   stepper->direction_pin = direction_pin;
   stepper->steps_per_revolution = steps_per_revolution;

   adv_ctl_timer_init(timer);
   adv_ctl_timer_pwm_init(timer, 1);
}

void stepper_set_speed(Stepper *stepper, uint32_t rpm) {
   uint32_t ticks_per_sec = CYCLE_WIDTH * stepper->steps_per_revolution * rpm;
   uint32_t prescaler = 60 * CLOCK_SPEED / ticks_per_sec;
   adv_ctl_timer_pwm_config(stepper->timer, prescaler, CYCLE_WIDTH);
   adv_ctl_timer_pwm_duty_cycle(1, 1, DUTY_CYCLE);
}

void stepper_set_enabled(Stepper *stepper, bool enable) {
   if (!enable) {
      return; // todo
   }

   adv_ctl_timer_pwm_start(stepper->timer);
}
