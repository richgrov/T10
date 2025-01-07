#include "timer.h"

#include <stdint-gcc.h>

#include "gpio.h"
#include "rcc.h"

typedef struct {
   volatile uint32_t control_1;
   volatile uint32_t control_2;
   volatile uint32_t slave_mode_control;
   volatile uint32_t interrupt_enable;
   volatile uint32_t status;
   volatile uint32_t event_generation;
   volatile uint32_t capture_compare_mode[2];
   volatile uint32_t capture_compare_enable;
   volatile uint32_t counter;
   volatile uint32_t prescaler;
   volatile uint32_t auto_reload;
   volatile uint32_t repitition_counter;
   volatile uint32_t capture_compare[4];
   volatile uint32_t break_dead_time;
   volatile uint32_t dma_control;
   volatile uint32_t dma_addr_full_transfer;
} AdvancedControlTimer;

#define TIM1 ((volatile AdvancedControlTimer *)0x40010000)
#define TIM8 ((volatile AdvancedControlTimer *)0x40010400)
#define GET_TIMER(n) (n == 1 ? TIM1 : n == 8 ? TIM8 : NULL)

#define TIM1_CH1_ALT_FUNC 1
#define TIM8_CH1_ALT_FUNC 3

typedef enum {
   C1_COUNTER_ENABLE = (1 << 0),
   C1_PRELOAD_AUTO_RELOAD = (1 << 7),
} Control1;

typedef enum {
   IE_UPDATE,
} InterruptEnable;

typedef enum {
   EG_UPDATE = (1 << 0),
} EventGeneration;

typedef enum {
   CCM_OUT_PRELOAD_ENABLE = (1 << 3),
   CCM_OUT_MODE_PWM_1 = (6 << 4),
   CCM_OUT_MODE_MASK = (7 << 4),
} CaptureCompareMode;

typedef enum {
   BDT_MAIN_OUTPUT_ENABLE = (1 << 15),
} BreakDeadTime;

void timer_init(uint8_t timer_num) {
   switch (timer_num) {
   case 1:
      rcc_apb2_enable(APB2_TIM1_ENABLE);
      break;
   case 8:
      rcc_apb2_enable(APB2_TIM8_ENABLE);
      break;
   }
}

void timer_pwm_init(uint8_t timer_num, uint8_t channel) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      switch (channel) {
      case 1:
         gpio_set_mode(GPIOA, 8, GPIO_MODE_ALT);
         gpio_set_af(GPIOA, 8, TIM1_CH1_ALT_FUNC);
         break;
      case 2:
         gpio_set_mode(GPIOA, 9, GPIO_MODE_ALT);
         gpio_set_af(GPIOA, 9, TIM1_CH1_ALT_FUNC);
         break;
      }
      timer = TIM1;
      break;
   case 8:
      gpio_set_mode(GPIOC, 6, GPIO_MODE_ALT);
      gpio_set_af(GPIOC, 6, TIM8_CH1_ALT_FUNC);
      timer = TIM8;
      break;
   }

   int reg_idx = (channel - 1) / 2;
   int reg_shift = (channel - 1) % 2 * 8;
   timer->capture_compare_mode[reg_idx] &= ~(CCM_OUT_MODE_MASK << reg_shift);
   timer->capture_compare_mode[reg_idx] |= (CCM_OUT_PRELOAD_ENABLE | CCM_OUT_MODE_PWM_1)
                                           << reg_shift;
   timer->capture_compare_enable |= (1 << 4 * (channel - 1));
   timer->break_dead_time |= BDT_MAIN_OUTPUT_ENABLE;
}

void timer_pwm_config(uint8_t timer_num, uint16_t prescaler, uint16_t frequency) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      timer = TIM1;
      break;
   case 8:
      timer = TIM8;
      break;
   }

   timer->prescaler = prescaler - 1;
   timer->auto_reload = frequency - 1;
}

void timer_pwm_duty_cycle(uint8_t timer_num, uint16_t channel, uint16_t duty_cycle) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      timer = TIM1;
      break;
   case 8:
      timer = TIM8;
      break;
   }

   timer->capture_compare[channel - 1] = duty_cycle;
}

void timer_pwm_start(uint8_t timer_num) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      timer = TIM1;
      break;
   case 8:
      timer = TIM8;
      break;
   }

   timer->event_generation |= EG_UPDATE;
   timer->control_1 |= C1_PRELOAD_AUTO_RELOAD;
   timer->control_1 |= C1_COUNTER_ENABLE;
}

void timer_enable_update_isr(uint8_t timer_num) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      timer = TIM1;
      break;
   case 8:
      timer = TIM8;
      break;
   }

   timer->interrupt_enable |= 3;
   timer->status = 0;
}

bool timer_is_tim1_update_now() {
   if ((TIM1->status & 1) != 0) {
      TIM1->status &= ~1;
      return true;
   }
   return false;
}
