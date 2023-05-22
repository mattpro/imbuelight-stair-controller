#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "config.h"




static const uint8_t pwm_hw_pins[16] = {
    PWM_CHANNEL_1_PIN, 
    PWM_CHANNEL_2_PIN,
    PWM_CHANNEL_3_PIN,
    PWM_CHANNEL_4_PIN,
    PWM_CHANNEL_5_PIN,
    PWM_CHANNEL_6_PIN,
    PWM_CHANNEL_7_PIN,
    PWM_CHANNEL_8_PIN,
    PWM_CHANNEL_9_PIN,
    PWM_CHANNEL_10_PIN,
    PWM_CHANNEL_11_PIN,
    PWM_CHANNEL_12_PIN,
    PWM_CHANNEL_13_PIN,
    PWM_CHANNEL_14_PIN,
    PWM_CHANNEL_15_PIN,
    PWM_CHANNEL_16_PIN
};

static uint pwm_slice[16];

void PWM_init(void)
{
    int i;

    for ( i = 0 ; i < 16 ; i ++ )
    {
        gpio_set_function(pwm_hw_pins[i], GPIO_FUNC_PWM); 
        pwm_slice[i] = pwm_gpio_to_slice_num(pwm_hw_pins[i]);
        pwm_set_wrap(pwm_slice[i], 0);
        pwm_set_chan_level(pwm_slice[i], i%2, 0);
        pwm_set_enabled(pwm_slice[i], true);
    }
}

void PWM_set_duty(uint8_t channel, uint16_t duty)
{
    pwm_set_chan_level(pwm_slice[channel], channel%2, duty);
}