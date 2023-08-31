#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "pwm.h"
#include "pwm_pio.h"
#include "config.h"
#include "my_i2c.h"
#include "io_exp.h"

#include "rtt/RTT/SEGGER_RTT.h"


#define STAIR_LIGHT_ON_TIME     5000 //* 1ms - 5 000 - 5s


typedef struct {
    bool light_on_flag;
    uint32_t light_off_counter;
    effect_dir_t dir;
} light_on_t;


typedef struct {
    uint8_t first_stair;
    uint8_t last_stair;
    uint8_t num_of_stairs;
    uint16_t max_pwm_duty;
} settings_t;


typedef struct {
    int step;                   // current effect step
    bool enable;                // enable effect
    effect_dir_t dir;           // direction DIR_DOWN_TO_UP or DIR_UP_TO_DOWN
    // settings
    uint16_t increment_step;    // speed of effect
    uint16_t wide;              // wide of effect
    turn_dir_t turn_dir;        // if enable effect goes on or off
} stair_effect_t;



volatile settings_t settings;
volatile light_on_t light_on;

volatile stair_effect_t effect_1;
volatile stair_effect_t effect_2;

struct repeating_timer main_timer;
struct repeating_timer io_pool_timer;

uint16_t counter = 0;


static uint32_t io_exp_pooling_counter = 0;
static volatile bool io_exp_pooling_flag = false;


void effect_1_start(effect_dir_t dir)
{
    effect_1.dir = dir;
    effect_1.step = 0;
    effect_1.turn_dir = TURN_ON;
    effect_1.enable = true;
}

void effect_1_end(effect_dir_t dir)
{
    effect_1.dir = dir;
    effect_1.step = 0;
    effect_1.turn_dir = TURN_OFF;
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
    io_exp_pooling_counter ++;
    if ( io_exp_pooling_counter > 250 )
    {
        io_exp_pooling_flag = true;
        io_exp_pooling_counter = 0;
    }

    if ( effect_1.enable )
    {
        uint8_t step_number;

        effect_1.step += effect_1.increment_step; 

        for ( int i = 0 ; i < 24 ; i ++ )
        {
            if ( effect_1.dir == DIR_UP_TO_DOWN )
            {
                step_number = i;
            }
            else
            {
                step_number = settings.num_of_stairs - i;
            }

            if ( effect_1.turn_dir == TURN_ON )
            {
                PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number,  -effect_1.wide*i + effect_1.step);
            }
            else
            {
                PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number,  settings.max_pwm_duty + effect_1.wide*i - effect_1.step);
            }
        }

        if ( ( -effect_1.wide*step_number > settings.max_pwm_duty ) &&  ( effect_1.turn_dir == TURN_ON ) )
        {
            effect_1.enable = false;
        }
        if ( ( (settings.max_pwm_duty + effect_1.wide*step_number ) < 0  ) &&  ( effect_1.turn_dir == TURN_OFF ) )
        {
            effect_1.enable = false;
        }
    }

    // if ( effect_2.enable )
    // {
    //     uint8_t step_number;
    //     effect_2.step += effect_2.increment_step;
    //     for ( int i = 0 ; i < 24 ; i ++ )
    //     {
    //         if ( effect_2.dir == DIR_UP_TO_DOWN )
    //         {
    //             step_number = i;
    //         }
    //         else
    //         {
    //             step_number = settings.num_of_stairs - i;
    //         }

    //         PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number,  settings.max_pwm_duty - ( -effect_2.wide*i + effect_2.step ) );
    //     }

    //     // // End effect condition
    //     // if ( ( -effect_2.wide*settings.last_stair + effect_2.step ) >= settings.max_pwm_duty )
    //     // {
    //     //     effect_2.enable = false;
    //     // }
    // }

    if ( light_on.light_on_flag == true )
    {
        light_on.light_off_counter ++;
        if ( light_on.light_off_counter > STAIR_LIGHT_ON_TIME )
        {
            SEGGER_RTT_WriteString(0,"Light exit\r\n");
            light_on.dir == DIR_UP_TO_DOWN ? effect_1_end(DIR_UP_TO_DOWN) : effect_1_end(DIR_DOWN_TO_UP);
            light_on.light_on_flag = false;
        }  
    }   

    return true;
}


void sens_top_enter(void)
{
    SEGGER_RTT_WriteString(0,"Sensor top enter\r\n");   
    light_on.light_off_counter = 0;
    if ( light_on.light_on_flag == false )
    {
        SEGGER_RTT_WriteString(0,"Light enter\r\n");
        light_on.light_on_flag = true;
        light_on.dir = DIR_UP_TO_DOWN;
        effect_1_start(light_on.dir);
    }  
}

void sens_top_exit(void)
{
    SEGGER_RTT_WriteString(0,"Sensor top exit\r\n");   
}

void sens_bottom_enter(void)
{
    SEGGER_RTT_WriteString(0,"Sensor bottom enter\r\n");
    light_on.light_off_counter = 0;
    if ( light_on.light_on_flag == false )
    {
        SEGGER_RTT_WriteString(0,"Light enter\r\n");
        light_on.light_on_flag = true;
        light_on.dir = DIR_DOWN_TO_UP;
        effect_1_start(light_on.dir);
    }  
}

void sens_bottom_exit(void)
{
    SEGGER_RTT_WriteString(0,"Sensor bottom exit\r\n"); 
}

bool led_state = false;

int main() 
{
    // const uint LED_PIN = 6;
    // bool state = true;
    // if (cyw43_arch_init()) {
    //     printf("Wi-Fi init failed");
    //     return -1;
    // }

    stdio_init_all();

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

    SEGGER_RTT_WriteString(0,"############################################\r\n");
    SEGGER_RTT_WriteString(0,"### IMBUE LIGHT STAIR CONTROLLER VER 1.1 ###\r\n");
    SEGGER_RTT_printf(0, "###         %s %s         ###\r\n", __DATE__, __TIME__);
    SEGGER_RTT_WriteString(0,"############################################\r\n");


    // STAIR Settings
    settings.first_stair = 0;
    settings.last_stair = 23;
    settings.num_of_stairs = 24;
    settings.max_pwm_duty = MAX_PWM_DUTY;

    // Effect 1 settings
    effect_1.enable = false;
    effect_1.increment_step = 10;
    effect_1.wide = 1000;
    effect_1.dir = DIR_DOWN_TO_UP;
    // Effect 2 settings
    effect_2.enable = false;
    effect_2.increment_step = 10;
    effect_2.wide = 1000;
    effect_2.dir = DIR_DOWN_TO_UP;


    PWM_HW_init();
    PWM_PIO_init();
    I2C_init();
    IO_EXP_init();
    IO_EXP_reg_event_sens_top_cbfunc(sens_top_enter, sens_top_exit);
    IO_EXP_reg_event_sens_bottom_cbfunc(sens_bottom_enter, sens_bottom_exit);
    add_repeating_timer_us(-1000, main_timer_callback, NULL, &main_timer);

    SEGGER_RTT_printf(0,"Boot ok\r\n");

    while(1)
    {
        static uint32_t counter = 0;

        if ( io_exp_pooling_flag == true)
        { 
            io_exp_pooling_flag = false;
            counter++;
            //SEGGER_RTT_printf(0,"pooling io exp. C: %4d\r\n", counter);
            IO_EXP_pooling();
        }
    }
}
