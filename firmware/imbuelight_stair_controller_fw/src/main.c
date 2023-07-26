#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "pwm.h"
#include "pwm_pio.h"
#include "config.h"
#include "my_i2c.h"




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
stair_effect_t effect_2;


struct repeating_timer main_timer;
bool main_timer_flag = false;

uint16_t counter = 0;


void effect_1_start(effect_dir_t dir)
{
    effect_1.dir = dir;
    effect_1.step = 0;
    effect_1.enable = true;
}

void effect_2_start(effect_dir_t dir)
{
    effect_2.dir = dir;
    effect_2.step = 0; 
    effect_2.enable = true;
}


bool main_timer_callback(struct repeating_timer *t) 
{


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
            effect_1.enable = false;

        }
    }


    if ( effect_2.enable )
    {
        uint8_t step_number;
        effect_2.step += effect_2.increment_step;
        for ( int i = 0 ; i < 24 ; i ++ )
        {
            if ( effect_2.dir == DIR_UP_TO_DOW )
            {
                step_number = i;
            }
            else
            {
                step_number = settings.num_of_stairs - i;
            }

            PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number,  settings.max_pwm_duty - ( -effect_2.wide*i + effect_2.step ) );
        }

        // // End effect condition
        // if ( ( -effect_2.wide*settings.last_stair + effect_2.step ) >= settings.max_pwm_duty )
        // {
        //     effect_2.enable = false;
        // }
    }








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

    stdio_init_all();

    printf("############################################\r\n");
    printf("### IMBUE LIGHT STAIR CONTROLLER VER 1.0 ###\r\n");
    printf("############################################\r\n");

    settings.first_stair = 0;
    settings.last_stair = 23;
    settings.num_of_stairs = 24;
    settings.max_pwm_duty = MAX_PWM_DUTY;

    effect_1.enable = true;
    effect_1.increment_step = 10;
    effect_1.wide = 1000;
    effect_1.dir = DIR_DOWN_TO_UP;

    effect_2.enable = false;
    effect_2.increment_step = 10;
    effect_2.wide = 1000;
    effect_2.dir = DIR_DOWN_TO_UP;

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

    add_repeating_timer_us(200, main_timer_callback, NULL, &main_timer);

    I2C_init();


    uint8_t io_state;
    while(1)
    {
        io_state = read_io_expander_state();
        printf("IO STATE: %02X\r\n", io_state);
        sleep_ms(100);
    }
    




    while(1)
    {

        printf("XXXX");

        effect_1_start(DIR_UP_TO_DOW);
        sleep_ms(1500);
        effect_1.enable = false;
        //effect_1.dir ^= 1;
        effect_2_start(DIR_UP_TO_DOW);
        sleep_ms(1500);
        effect_2.enable = false;
        if ( main_timer_flag )
        {
            main_timer_flag = false;    

            // for ( int i = 0 ; i < 24 ; i ++ )
            // {
            //     PWM_set_duty_in_channel_with_gamma( (pwm_channel_t)i, counter );
            // }
    
            // counter += 1;



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
