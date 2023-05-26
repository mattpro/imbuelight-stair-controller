#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "pwm.h"
#include "pwm_pio.h"
#include "config.h"




typedef struct {
    uint8_t first_stair;
    uint8_t last_stair;
    uint8_t num_of_stairs;

    uint16_t max_pwm_duty;



} settings_t;




typedef struct {
    int step;
    bool enable;
    effect_dir_t dir;
    // settings
    uint16_t increment_step;
    uint16_t wide;



} stair_effect_t;

settings_t settings;

stair_effect_t effect_1;



struct repeating_timer main_timer;
bool main_timer_flag = false;

uint16_t counter = 0;


bool main_timer_callback(struct repeating_timer *t) 
{
    main_timer_flag = true;
    return true;
}


int main() 
{
    // const uint LED_PIN = 6;
    // bool state = true;
    // if (cyw43_arch_init()) {
    //     printf("Wi-Fi init failed");
    //     return -1;
    // }


    settings.first_stair = 0;
    settings.last_stair = 23;
    settings.num_of_stairs = 24;
    settings.max_pwm_duty = MAX_PWM_DUTY;

    effect_1.enable = true;
    effect_1.increment_step = 10;
    effect_1.wide = 1000;
    effect_1.dir = DIR_DOWN_TO_UP;

    gpio_init(PWM_CHANNEL_1_PIN);
    gpio_set_dir(PWM_CHANNEL_1_PIN, GPIO_OUT);
   
    for( int i = 0 ; i < 20 ; i ++ )
    {
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        gpio_put(PWM_CHANNEL_1_PIN, 1 );
        sleep_ms(25);
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        gpio_put(PWM_CHANNEL_1_PIN, 0 );
        sleep_ms(25);
    }

    PWM_HW_init();
    PWM_PIO_init();

    add_repeating_timer_us(1000, main_timer_callback, NULL, &main_timer);



    while(1)
    {
        if ( main_timer_flag )
        {
            main_timer_flag = false;    

            // for ( int i = 0 ; i < 24 ; i ++ )
            // {
            //     PWM_set_duty_in_channel_with_gamma( (pwm_channel_t)i, counter );
            // }
    
            // counter += 1;

            if ( effect_1.enable )
            {
                uint8_t step_number;
                effect_1.step += effect_1.increment_step;
                for ( int i = 0 ; i < 24 ; i ++ )
                {
                    if ( effect_1.dir == DIR_UP_TO_DOW )
                    {
                        step_number = i;
                    }
                    else
                    {
                        step_number = settings.num_of_stairs - i;
                    }

                    PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number,  -effect_1.wide*i + effect_1.step);
                }

                // End effect condition
                if ( ( -effect_1.wide*settings.last_stair + effect_1.step ) >= settings.max_pwm_duty )
                {
                    //effect_1.enable = false;
                    effect_1.dir ^= 1;
                    effect_1.step = 0;
                }

            }
            else
            {
                for ( int i = 0 ; i < 24 ; i ++ )
                {
                    PWM_set_duty_in_channel_with_gamma((pwm_channel_t)i,  0 );
                }
            }

            







        }

        if ( counter > 0x3FFF )
        {
            counter = 0;
//          state ^= true;
//          gpio_put(PWM_CHANNEL_1_PIN, state );
        }

        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(10);
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        // sleep_ms(10);
    }
}
