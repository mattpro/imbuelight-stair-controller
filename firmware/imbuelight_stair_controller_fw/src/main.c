#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
#include "pwm.h"
#include "config.h"


// Write `period` to the input shift register
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}



int main() 
{
    const uint LED_PIN = 6;

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

   // gpio_init(LED_PIN);
   // gpio_set_dir(LED_PIN, GPIO_OUT);

   
    

    uint offset_pio0 = pio_add_program(PIO_CHANNEL_17_20, &pwm_program);
    pwm_program_init(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_1, offset_pio0, PWM_CHANNEL_17_PIN);
    pwm_program_init(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_2, offset_pio0, PWM_CHANNEL_18_PIN);
    pwm_program_init(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_3, offset_pio0, PWM_CHANNEL_19_PIN);
    pwm_program_init(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_4, offset_pio0, PWM_CHANNEL_20_PIN);
    pio_pwm_set_period(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_1, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_2, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_3, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_4, (1u << 16) - 1);


    uint offset_pio1 = pio_add_program(PIO_CHANNEL_21_24, &pwm_program);
    pwm_program_init(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_1, offset_pio1, PWM_CHANNEL_21_PIN);
    pwm_program_init(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_2, offset_pio1, PWM_CHANNEL_22_PIN);
    pwm_program_init(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_3, offset_pio1, PWM_CHANNEL_23_PIN);
    pwm_program_init(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_4, offset_pio1, PWM_CHANNEL_24_PIN);
    pio_pwm_set_period(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_1, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_2, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_3, (1u << 16) - 1);
    pio_pwm_set_period(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_4, (1u << 16) - 1);


    int level = 0;


    PWM_init();


    while (true) 
    {
        pio_pwm_set_level(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_1, 5000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_2, 10000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_3, 15000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_17_20, PIO_STATE_MACHINE_4, 20000 + level*level);

        pio_pwm_set_level(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_1, 25000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_2, 30000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_3, 35000 + level*level);
        pio_pwm_set_level(PIO_CHANNEL_21_24, PIO_STATE_MACHINE_4, 40000 + level*level);


        for ( int i = 0 ; i < 16 ; i ++)
        {
          PWM_set_duty(i, level*50 );  
        }
        

        level = (level + 1) % 256;
        sleep_ms(10);


        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(250);
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        // sleep_ms(250);

        // gpio_put(LED_PIN, 1);
        // sleep_ms(25);
        // gpio_put(LED_PIN, 0);
        // sleep_ms(25);
    }

}
