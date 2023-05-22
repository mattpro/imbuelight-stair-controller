#ifndef _PWM_H_
#define _PWM_H_

#include "pico/stdlib.h"

void PWM_init(void);
void PWM_set_duty(uint8_t channel, uint16_t duty);

#endif