#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pwm.pio.h"
#include "pwm.h"
#include "pwm_pio.h"
#include "config.h"

#define CLK_DIV 1

// Write `period` to the input shift register
static void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) 
{
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}


void PWM_PIO_init(void)
{   
    uint offset_pio0 = pio_add_program(PIO_CHANNEL_5_8, &pwm_program);
    pwm_program_init(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_1, offset_pio0, CLK_DIV, PWM_CHANNEL_5_PIN);
    pwm_program_init(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_2, offset_pio0, CLK_DIV, PWM_CHANNEL_6_PIN);
    pwm_program_init(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_3, offset_pio0, CLK_DIV, PWM_CHANNEL_7_PIN);
    pwm_program_init(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_4, offset_pio0, CLK_DIV, PWM_CHANNEL_8_PIN);

    uint offset_pio1 = pio_add_program(PIO_CHANNEL_9_12, &pwm_program);
    pwm_program_init(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_1, offset_pio1, CLK_DIV, PWM_CHANNEL_9_PIN);
    pwm_program_init(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_2, offset_pio1, CLK_DIV, PWM_CHANNEL_10_PIN);
    pwm_program_init(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_3, offset_pio1, CLK_DIV, PWM_CHANNEL_11_PIN);
    pwm_program_init(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_4, offset_pio1, CLK_DIV, PWM_CHANNEL_12_PIN);   
    

    pio_pwm_set_period(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_1, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_2, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_3, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_4, MAX_PWM_DUTY );

    pio_pwm_set_period(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_1, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_2, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_3, MAX_PWM_DUTY );
    pio_pwm_set_period(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_4, MAX_PWM_DUTY );


    PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_1, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_2, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_3, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_4, 0 );

    PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_1, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_2, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_3, 0 );
    PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_4, 0 );
}


void PWM_PIO_set_level(PIO pio, uint sm, uint32_t level) 
{
    pio_sm_put_blocking(pio, sm, level);
}
