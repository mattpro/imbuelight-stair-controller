#ifndef _PWM_H_
#define _PWM_H_

#include "pico/stdlib.h"

#define MAX_PWM_DUTY 0x3FFF


typedef enum 
{ 
    CH_1, 
    CH_2, 
    CH_3,
    CH_4, 
    CH_5, 
    CH_6,
    CH_7, 
    CH_8, 
    CH_9,
    CH_10, 
    CH_11, 
    CH_12, 
    CH_13, 
    CH_14, 
    CH_15,
    CH_16, 
    CH_17, 
    CH_18,
    CH_19, 
    CH_20, 
    CH_21,
    CH_22, 
    CH_23, 
   // CH_24,
} pwm_channel_t;


void PWM_HW_init(void);
void PWM_HW_set_duty(uint8_t channel, uint16_t duty);

void PWM_set_duty_in_channel(pwm_channel_t channel, uint16_t duty);
void PWM_set_duty_in_channel_with_gamma(pwm_channel_t channel, int duty);
void PWM_set_duty_in_channel_with_gamma_max_duty(pwm_channel_t channel, int duty, int max_duty);

#endif