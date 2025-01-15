#include "stepper.h"

#include <stdint-gcc.h>

#include "config.h"
#include "firmware/gpio.h"
#include "firmware/nvic.h"
#include "firmware/systick.h"
#include "firmware/timer.h"
#include "firmware/usart.h"
#include "math.h"

// Set to powers of 2 to improve integer division performance
#define CYCLE_WIDTH 1024
#define DUTY_CYCLE 512

StepperController *steppers[4];

void stepper_update_isr(int timer_num) {
   StepperController *stepper = steppers[timer_num - 1];
   if (stepper->direction == FORWARD) {
      ++stepper->position;
   } else {
      --stepper->position;
   }
}

void stepper_tim1_update_isr(void) {
   if (timer_is_tim1_update_now()) {
      stepper_update_isr(1);
   }
}

void stepper_init(StepperController *stepper) {
   switch (stepper->timer) {
   case 1:
      nvic_enable(ISR_TIM1_UP_TIM10);
      break;
   }

   steppers[stepper->timer - 1] = stepper;
   timer_init(stepper->timer);
   timer_pwm_init(stepper->timer, 1);
   timer_pwm_duty_cycle(stepper->timer, 1, DUTY_CYCLE);
   timer_enable_update_isr(stepper->timer);
   gpio_set_mode(stepper->direction_gpio, stepper->direction_pin, GPIO_MODE_OUTPUT);
}

uint16_t stepper_rpm(StepperController *stepper, uint32_t now) {
   (void)now;
   stepper->direction = stepper->position < stepper->target ? FORWARD : BACKWARD;

   if (stepper->position == stepper->target) {
      return 0;
   }

   return stepper->max_rpm;
}

void stepper_update(StepperController *stepper) {
   uint32_t now = systick_time();
   uint32_t ticks_per_min = CYCLE_WIDTH * stepper->steps_per_revolution * stepper_rpm(stepper, now);

   usart_write_str(2, "pos: ");
   usart_write_u32(2, stepper->position);
   usart_write_str(2, "tar: ");
   usart_write_u32(2, stepper->target);
   usart_write_str(2, "\r\n");

   if (ticks_per_min == 0) {
      timer_pwm_stop(1);
      stepper->enabled = false;
      return;
   }

   uint16_t prescaler = 60 * CLOCK_SPEED / ticks_per_min;
   timer_pwm_config(stepper->timer, prescaler, CYCLE_WIDTH);
   bool direction_pin_high = stepper->direction == BACKWARD;
   gpio_write_pin(stepper->direction_gpio, stepper->direction_pin, direction_pin_high);

   if (!stepper->enabled) {
      stepper->enabled = true;
      timer_pwm_start(stepper->timer);
   }
}
