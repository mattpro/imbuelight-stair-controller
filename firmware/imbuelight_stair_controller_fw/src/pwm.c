#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "pwm_pio.h"
#include "pwm.h"
#include "config.h"


static const uint8_t pwm_hw_pins[16] = {
    PWM_CHANNEL_1_PIN,
    PWM_CHANNEL_2_PIN,
    PWM_CHANNEL_24_PIN,
    PWM_CHANNEL_23_PIN,
    PWM_CHANNEL_3_PIN,
    PWM_CHANNEL_4_PIN,
    PWM_CHANNEL_22_PIN,
    PWM_CHANNEL_21_PIN,
    PWM_CHANNEL_20_PIN, 
    PWM_CHANNEL_19_PIN,
    PWM_CHANNEL_18_PIN,
    PWM_CHANNEL_17_PIN,
    PWM_CHANNEL_16_PIN,
    PWM_CHANNEL_15_PIN,
    PWM_CHANNEL_14_PIN,
    PWM_CHANNEL_13_PIN,
};

static volatile uint pwm_slice[16];


// PWM duty from 0 to 65535
// PWM frequency = 125 MHz / (MAX_DUTY * CLK_DIV)

void PWM_HW_init(void)
{
    int i;
    
    for ( i = 0 ; i < 16 ; i ++ )
    {
        gpio_set_function(pwm_hw_pins[i], GPIO_FUNC_PWM); 
        pwm_slice[i] = pwm_gpio_to_slice_num(pwm_hw_pins[i]);
        pwm_set_wrap(pwm_slice[i], MAX_PWM_DUTY);
        pwm_set_clkdiv(pwm_slice[i], 3);
        pwm_set_enabled(pwm_slice[i], true);
    }
}


void PWM_HW_set_duty(uint8_t channel, uint16_t duty)
{
    pwm_set_chan_level(pwm_slice[channel], channel%2, duty);
}


void PWM_set_duty_in_channel(pwm_channel_t channel, uint16_t duty)
{
    switch(channel)
    {
        case CH_1:
            pwm_set_chan_level(0, PWM_CHAN_A, duty);   
        break;
        case CH_2:
            pwm_set_chan_level(0, PWM_CHAN_B, duty);   
        break;
        case CH_24:
            pwm_set_chan_level(1, PWM_CHAN_A, duty);   
        break;
        case CH_23:
            pwm_set_chan_level(1, PWM_CHAN_B, duty);   
        break;
        case CH_3:
            pwm_set_chan_level(2, PWM_CHAN_A, duty);   
        break;
        case CH_4:
            pwm_set_chan_level(2, PWM_CHAN_B, duty);   
        break;
        case CH_22:
            pwm_set_chan_level(3, PWM_CHAN_A, duty);   
        break;
        case CH_21:
            pwm_set_chan_level(3, PWM_CHAN_B, duty);   
        break;
        case CH_20:
            pwm_set_chan_level(4, PWM_CHAN_A, duty);   
        break;
        case CH_19:
            pwm_set_chan_level(4, PWM_CHAN_B, duty);   
        break;
        case CH_18:
            pwm_set_chan_level(5, PWM_CHAN_A, duty);   
        break;
        case CH_17:
            pwm_set_chan_level(5, PWM_CHAN_B, duty);   
        break;
        case CH_16:
            pwm_set_chan_level(6, PWM_CHAN_A, duty);   
        break;
        case CH_15:
            pwm_set_chan_level(6, PWM_CHAN_B, duty);   
        break;
        case CH_14:
            pwm_set_chan_level(7, PWM_CHAN_A, duty);   
        break;
        case CH_13:
            pwm_set_chan_level(7, PWM_CHAN_B, duty);   
        break;
        case CH_5:
            PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_1, duty );  
        break;
        case CH_6:
            PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_2, duty );  
        break;
        case CH_7:
            PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_3, duty );  
        break;
        case CH_8:
            PWM_PIO_set_level(PIO_CHANNEL_5_8, PIO_STATE_MACHINE_4, duty );  
        break;
        case CH_9:
            PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_1, duty );  
        break;
        case CH_10:
            PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_2, duty );  
        break;
        case CH_11:
            PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_3, duty );  
        break;
        case CH_12:
            PWM_PIO_set_level(PIO_CHANNEL_9_12, PIO_STATE_MACHINE_4, duty );  
        break;
    }
}


void PWM_set_duty_in_channel_with_gamma(pwm_channel_t channel, int duty)
{
    uint16_t gamma_duty = 0;

    if ( duty > MAX_PWM_DUTY )
    {
        gamma_duty = MAX_PWM_DUTY;
    }
    else if ( duty < 0 )
    {
        gamma_duty = 0;
    }
    else
    {
        gamma_duty = (uint16_t)(powf( (float)duty / (float)MAX_PWM_DUTY, GAMMA_FACTOR ) * MAX_PWM_DUTY + 0.5);
    }

    PWM_set_duty_in_channel(channel, gamma_duty);
}