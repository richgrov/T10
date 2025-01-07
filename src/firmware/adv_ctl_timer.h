#ifndef REMIX_FIRMWARE_ADV_CTL_TIMER_H_
#define REMIX_FIRMWARE_ADV_CTL_TIMER_H_

#include <stdbool.h>
#include <stdint-gcc.h>

void adv_ctl_timer_init(uint8_t timer_num);

void adv_ctl_timer_pwm_init(uint8_t timer_num, uint8_t channel);

void adv_ctl_timer_pwm_config(uint8_t timer_num, uint16_t prescaler, uint16_t frequency);

void adv_ctl_timer_pwm_duty_cycle(uint8_t timer_num, uint16_t channel, uint16_t duty_cycle);

void adv_ctl_timer_pwm_start(uint8_t timer_num);

void adv_ctl_timer_enable_update_isr(uint8_t timer_num);

bool adv_ctl_timer_is_tim1_update_now();

#endif // !REMIX_FIRMWARE_ADV_CTL_TIMER_H_
