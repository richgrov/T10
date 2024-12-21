#include "adv_ctl_timer.h"

#include <stdint-gcc.h>

#include "gpio.h"
#include "rcc.h"

typedef struct {
   uint32_t control_1;
   uint32_t control_2;
   uint32_t slave_mode_control;
   uint32_t interrupt_enable;
   uint32_t status;
   uint32_t event_generation;
   uint32_t capture_compare_mode_1;
   uint32_t capture_compare_mode_2;
   uint32_t capture_compare_enable;
   uint32_t counter;
   uint32_t prescaler;
   uint32_t auto_reload;
   uint32_t repitition_counter;
   uint32_t capture_compare_1;
   uint32_t capture_compare_2;
   uint32_t capture_compare_3;
   uint32_t capture_compare_4;
   uint32_t break_dead_time;
   uint32_t dma_control;
   uint32_t dma_addr_full_transfer;
} AdvancedControlTimer;

#define TIM1 ((volatile AdvancedControlTimer *)0x40010000)
#define TIM8 ((volatile AdvancedControlTimer *)0x40010400)

#define TIM1_CH1_ALT_FUNC 1
#define TIM8_CH1_ALT_FUNC 3

typedef enum {
   C1_COUNTER_ENABLE = (1 << 0),
   C1_PRELOAD_AUTO_RELOAD = (1 << 7),
} Control1;

typedef enum {
   EG_UPDATE = (1 << 0),
} EventGeneration;

typedef enum {
   CCM1_OUT_COMPARE_1_PRELOAD_ENABLE = (1 << 3),
   CCM1_OUT_COMPARE_1_MODE_PWM_1 = (6 << 4),
   CCM1_OUT_COMPARE_1_MODE_MASK = (7 << 4),
} CaptureCompareMode1;

typedef enum {
   CCE_OUTPUT_1_ENABLE = (1 << 0),
} CaptureCompareEnable;

typedef enum {
   BDT_MAIN_OUTPUT_ENABLE = (1 << 15),
} BreadDeadTime;

void adv_ctl_timer_init(uint8_t timer_num) {
   switch (timer_num) {
   case 1:
      rcc_apb2_enable(APB2_TIM1_ENABLE);
      break;
   case 8:
      rcc_apb2_enable(APB2_TIM8_ENABLE);
      break;
   }
}

void adv_ctl_timer_pwm_init(uint8_t timer_num) {
   volatile AdvancedControlTimer *timer;

   switch (timer_num) {
   case 1:
      gpio_set_mode(GPIOA, 8, GPIO_MODE_ALT);
      gpio_set_af(GPIOA, 8, TIM1_CH1_ALT_FUNC);
      timer = TIM1;
      break;
   case 8:
      gpio_set_mode(GPIOC, 6, GPIO_MODE_ALT);
      gpio_set_af(GPIOC, 6, TIM8_CH1_ALT_FUNC);
      timer = TIM8;
      break;
   }

   timer->capture_compare_mode_1 &= ~CCM1_OUT_COMPARE_1_MODE_MASK;
   timer->capture_compare_mode_1 |=
      CCM1_OUT_COMPARE_1_PRELOAD_ENABLE | CCM1_OUT_COMPARE_1_MODE_PWM_1;
   timer->capture_compare_enable |= CCE_OUTPUT_1_ENABLE;
   timer->break_dead_time |= BDT_MAIN_OUTPUT_ENABLE;
}

void adv_ctl_timer_pwm_config(
   uint8_t timer_num, uint16_t prescaler, uint16_t frequency, uint16_t duty_cycle
) {
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
   timer->capture_compare_1 = duty_cycle;
}

void adv_ctl_timer_pwm_start(uint8_t timer_num) {
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
