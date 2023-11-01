#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
//#include "pico/cyw43_arch.h"
#include "pwm.h"
#include "pwm_pio.h"
#include "config.h"
#include "my_i2c.h"
#include "io_exp.h"
#include "utils.h"
#include "rtt/RTT/SEGGER_RTT.h"


#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "server_common.h"


#define HEARTBEAT_PERIOD_MS 1000


typedef struct {
    bool light_on_flag;
    bool light_on_from_top_flag;
    bool light_on_from_bottom_flag;
    uint32_t light_off_counter;
    effect_dir_t dir;
} light_on_t;


typedef struct {
    uint8_t num_of_stairs;
    uint16_t max_pwm_duty;
    uint32_t stair_light_on_time_ms;
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

void effect_2_start(void)
{
    effect_2.step = 0;
    effect_2.turn_dir = TURN_ON;
    effect_2.enable = true;
}

void effect_2_end(void)
{
    effect_2.step = 0;
    effect_2.turn_dir = TURN_OFF;
    effect_2.enable = true;
}

bool main_timer_callback(struct repeating_timer *t) // each 2ms
{
    io_exp_pooling_counter ++;
    if ( io_exp_pooling_counter > 50 )
    {
        io_exp_pooling_flag = true;
        io_exp_pooling_counter = 0;
    }

    if ( effect_1.enable )
    {
        uint8_t step_number;
        int calculate_duty;

        effect_1.step += effect_1.increment_step; 

        SEGGER_RTT_WriteString(0,"------------------------------\r\n");

        // Update each step PWM
        for ( int i = 0 ; i < settings.num_of_stairs ; i ++ )
        {
            if      ( effect_1.dir == DIR_UP_TO_DOWN )  step_number = i;
            else if ( effect_1.dir == DIR_DOWN_TO_UP )  step_number = settings.num_of_stairs - i - 1;
            
            if      ( effect_1.turn_dir == TURN_ON  )   calculate_duty = -effect_1.wide*i + effect_1.step;
            else if ( effect_1.turn_dir == TURN_OFF )   calculate_duty = settings.max_pwm_duty + effect_1.wide*i - effect_1.step;

            PWM_set_duty_in_channel_with_gamma((pwm_channel_t)step_number, calculate_duty);

            SEGGER_RTT_printf(0, "[%2d]=%d\r\n", i, calculate_duty);
        }

        // Check ending condition
        if ( ( effect_1.turn_dir == TURN_ON  ) && ( calculate_duty > settings.max_pwm_duty ) )
        {
            SEGGER_RTT_WriteString(0,"Effect animation disable after turn on\r\n");
            effect_1.enable = false;
        }
        else if ( ( effect_1.turn_dir == TURN_OFF ) && ( calculate_duty < 0 ) )
        {
            SEGGER_RTT_WriteString(0,"Effect animation disable after turn off\r\n");
            effect_1.enable = false;
            light_on.light_on_from_top_flag = false; 
            light_on.light_on_from_bottom_flag = false; 
        }
    }

    if ( effect_2.enable )
    {
        effect_2.step += effect_2.increment_step;

        for ( int i = 0 ; i < settings.num_of_stairs ; i ++ )
        {
            if ( effect_2.turn_dir == TURN_ON )
            {
                PWM_set_duty_in_channel_with_gamma((pwm_channel_t)i,  effect_2.step );
            }
            else if ( effect_2.turn_dir == TURN_OFF )
            {
                PWM_set_duty_in_channel_with_gamma((pwm_channel_t)i,  settings.max_pwm_duty - effect_2.step );
            }
        }

        // End effect condition
        if ( ( effect_2.turn_dir == TURN_ON ) && (  effect_2.step > settings.max_pwm_duty ) )
        {
            effect_2.enable = false;
        }
        if ( ( effect_2.turn_dir == TURN_OFF ) && (  effect_2.step < 0 ) )
        {
            effect_2.enable = false;
        }
    }

    if ( ( light_on.light_on_flag == true ) && ( ( light_on.light_on_from_top_flag == true ) || ( light_on.light_on_from_bottom_flag == true ) ) )
    {
        light_on.light_off_counter ++;
        if ( light_on.light_off_counter > settings.stair_light_on_time_ms/2 ) // time diveded by 2 because loop interval is 2ms
        {
            SEGGER_RTT_WriteString(0,"Light exit\r\n");
            light_on.dir == DIR_UP_TO_DOWN ? effect_1_end(DIR_UP_TO_DOWN) : effect_1_end(DIR_DOWN_TO_UP);
            //effect_2_end();
            light_on.light_on_flag = false;
            light_on.light_off_counter = 0;
        }  
    }   

    return true;
}


void sens_top_enter(void)
{
    SEGGER_RTT_WriteString(0,"Sensor top enter\r\n");   
    light_on.light_off_counter = 0;
    light_on.light_on_from_top_flag = false;
    if ( ( light_on.light_on_from_top_flag == false ) && ( light_on.light_on_flag == false ) )
    {
        SEGGER_RTT_WriteString(0,"Light enter\r\n");
        light_on.light_on_flag = true;
        light_on.dir = DIR_UP_TO_DOWN;
        effect_1_start(light_on.dir);
        //effect_2_start();
    }  
}

void sens_top_exit(void)
{
    light_on.light_on_from_top_flag = true;
    SEGGER_RTT_WriteString(0,"Sensor top exit\r\n");   
}

void sens_bottom_enter(void)
{
    SEGGER_RTT_WriteString(0,"Sensor bottom enter\r\n");
    light_on.light_off_counter = 0;
    light_on.light_on_from_bottom_flag = false;
    if ( ( light_on.light_on_from_bottom_flag == false ) && ( light_on.light_on_flag == false ) )
    {
        SEGGER_RTT_WriteString(0,"Light enter\r\n");
        light_on.light_on_flag = true;
        light_on.dir = DIR_DOWN_TO_UP;
        effect_1_start(light_on.dir);
        //effect_2_start();
    }  
}

void sens_bottom_exit(void)
{
    light_on.light_on_from_bottom_flag = true;
    SEGGER_RTT_WriteString(0,"Sensor bottom exit\r\n"); 
}

bool led_state = false;


// Related to bluetooth
static btstack_timer_source_t heartbeat;
static btstack_packet_callback_registration_t hci_event_callback_registration;

static void heartbeat_handler(struct btstack_timer_source *ts) 
{
    static uint32_t counter = 0;
    counter++;

    // // Update the temp every 10s
    // if (counter % 10 == 0) {
    //     poll_temp();
    //     if (le_notification_enabled) {
    //         att_server_request_can_send_now_event(con_handle);
    //     }
    // }

    // Invert the led
    static int led_on = true;
    led_on = !led_on;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);

    // Restart timer
    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
}



int main() 
{
    stdio_init_all();
    start_blink_led();

    SEGGER_RTT_WriteString(0,"############################################\r\n");
    SEGGER_RTT_WriteString(0,"### IMBUE LIGHT STAIR CONTROLLER VER 1.1 ###\r\n");
    SEGGER_RTT_printf(0, "###         %s %s         ###\r\n", __DATE__, __TIME__);
    SEGGER_RTT_WriteString(0,"############################################\r\n");


    // STAIR Settings
    settings.num_of_stairs = 17;
    settings.max_pwm_duty = MAX_PWM_DUTY;
    settings.stair_light_on_time_ms = 5000;

    // Effect 1 settings
    effect_1.enable = false;
    effect_1.increment_step = 40;
    effect_1.wide = 1000;
    effect_1.dir = DIR_DOWN_TO_UP;

    // Effect 2 settings
    effect_2.enable = false;
    effect_2.increment_step = 10;
    effect_2.dir = DIR_DOWN_TO_UP;


    PWM_HW_init();
    PWM_PIO_init();
    I2C_init();
    IO_EXP_init();
    IO_EXP_reg_event_sens_top_cbfunc(sens_top_enter, sens_top_exit);
    IO_EXP_reg_event_sens_bottom_cbfunc(sens_bottom_enter, sens_bottom_exit);
    add_repeating_timer_us(-2000, main_timer_callback, NULL, &main_timer);

    SEGGER_RTT_printf(0,"Boot ok\r\n");


// Related to bluetooth

// initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
    if (cyw43_arch_init()) 
    {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }

    l2cap_init();
    sm_init();

    att_server_init(profile_data, att_read_callback, att_write_callback);    

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT event
    att_server_register_packet_handler(packet_handler);

    // set one-shot btstack timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);

    // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);
    
   // btstack_run_loop_execute();


    while(1)
    {
        static uint32_t counter = 0;

        if ( io_exp_pooling_flag == true)
        { 
            io_exp_pooling_flag = false;
            counter++;
            SEGGER_RTT_printf(0,"pooling io exp. C: %4d enable:%d light off counter: %d\r\n", counter, effect_1.enable, light_on.light_off_counter);
            IO_EXP_pooling();
        }
    }
}
