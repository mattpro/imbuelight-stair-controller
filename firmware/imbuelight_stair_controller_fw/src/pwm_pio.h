#ifndef _PWM_PIO_H_
#define _PWM_PIO_H_

#include <stdint.h>
#include "pwm.pio.h"

#define PIO_STATE_MACHINE_1   0
#define PIO_STATE_MACHINE_2   1
#define PIO_STATE_MACHINE_3   2
#define PIO_STATE_MACHINE_4   3


void PWM_PIO_set_level(PIO pio, uint sm, uint32_t level);
void PWM_PIO_init(void);

#endif