#include "pico/stdlib.h"
#include "utils.h"
#include "config.h"



void start_blink_led(void)
{
    gpio_init(PWM_CHANNEL_1_PIN);
    gpio_set_dir(PWM_CHANNEL_1_PIN, GPIO_OUT);
    for( int i = 0 ; i < 20 ; i ++ )
    {
        //SEGGER_RTT_printf(0, "Counter: %d\r\n", i);
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        gpio_put(PWM_CHANNEL_1_PIN, 1 );
        sleep_ms(25);
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        gpio_put(PWM_CHANNEL_1_PIN, 0 );
        sleep_ms(25);
    }
}