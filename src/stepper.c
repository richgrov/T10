#include "stepper.h"

#include "config.h"
#include "firmware/timer.h"

// Set to powers of 2 to improve integer division performance
#define CYCLE_WIDTH 1024
#define DUTY_CYCLE 512

StepperController *steppers[4];

void stepper_update_isr(int timer_num) {
   StepperController *stepper = steppers[timer_num - 1];
   ++stepper->position;
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

   timer_init(stepper->timer);
   timer_pwm_init(stepper->timer, 1);
   timer_pwm_duty_cycle(stepper->timer, 1, DUTY_CYCLE);
   timer_enable_update_isr(stepper->timer);
   steppers[stepper->timer - 1] = stepper;
}

uint32_t stepper_speed(StepperController *stepper, uint32_t now) {
   (void)stepper;
   (void)now;
   return 0;
}

void stepper_update(StepperController *stepper) {
   (void)stepper;
}
