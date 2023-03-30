#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pwm.pio.h"
#include "pico/cyw43_arch.h"
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

    gpio_set_function(PWM_CHANNEL_1_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_2_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_3_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_4_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_5_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_6_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_7_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_8_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_9_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_10_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_11_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_12_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_13_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_14_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_15_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_CHANNEL_16_PIN, GPIO_FUNC_PWM);


    uint slice_num_ch1 = pwm_gpio_to_slice_num(PWM_CHANNEL_1_PIN);
    uint slice_num_ch2 = pwm_gpio_to_slice_num(PWM_CHANNEL_2_PIN);
    uint slice_num_ch3 = pwm_gpio_to_slice_num(PWM_CHANNEL_3_PIN);
    uint slice_num_ch4 = pwm_gpio_to_slice_num(PWM_CHANNEL_4_PIN);
    uint slice_num_ch5 = pwm_gpio_to_slice_num(PWM_CHANNEL_5_PIN);
    uint slice_num_ch6 = pwm_gpio_to_slice_num(PWM_CHANNEL_6_PIN);
    uint slice_num_ch7 = pwm_gpio_to_slice_num(PWM_CHANNEL_7_PIN);
    uint slice_num_ch8 = pwm_gpio_to_slice_num(PWM_CHANNEL_8_PIN);
    uint slice_num_ch9 = pwm_gpio_to_slice_num(PWM_CHANNEL_9_PIN);
    uint slice_num_ch10 = pwm_gpio_to_slice_num(PWM_CHANNEL_10_PIN);
    uint slice_num_ch11 = pwm_gpio_to_slice_num(PWM_CHANNEL_11_PIN);
    uint slice_num_ch12 = pwm_gpio_to_slice_num(PWM_CHANNEL_12_PIN);
    uint slice_num_ch13 = pwm_gpio_to_slice_num(PWM_CHANNEL_13_PIN);
    uint slice_num_ch14 = pwm_gpio_to_slice_num(PWM_CHANNEL_14_PIN);
    uint slice_num_ch15 = pwm_gpio_to_slice_num(PWM_CHANNEL_15_PIN);
    uint slice_num_ch16 = pwm_gpio_to_slice_num(PWM_CHANNEL_16_PIN);


    pwm_set_wrap(slice_num_ch1, 1600);
    pwm_set_wrap(slice_num_ch2, 1600);
    pwm_set_wrap(slice_num_ch3, 1600);
    pwm_set_wrap(slice_num_ch4, 1600);
    pwm_set_wrap(slice_num_ch5, 1600);
    pwm_set_wrap(slice_num_ch6, 1600);
    pwm_set_wrap(slice_num_ch7, 1600);
    pwm_set_wrap(slice_num_ch8, 1600);
    pwm_set_wrap(slice_num_ch9, 1600);
    pwm_set_wrap(slice_num_ch10, 1600);
    pwm_set_wrap(slice_num_ch11, 1600);
    pwm_set_wrap(slice_num_ch12, 1600);
    pwm_set_wrap(slice_num_ch13, 1600);
    pwm_set_wrap(slice_num_ch14, 1600);
    pwm_set_wrap(slice_num_ch15, 1600);
    pwm_set_wrap(slice_num_ch16, 1600);

    pwm_set_chan_level(slice_num_ch1, PWM_CHAN_A, 100);
    pwm_set_chan_level(slice_num_ch2, PWM_CHAN_B, 200);
    pwm_set_chan_level(slice_num_ch3, PWM_CHAN_A, 300);
    pwm_set_chan_level(slice_num_ch4, PWM_CHAN_B, 400);
    pwm_set_chan_level(slice_num_ch5, PWM_CHAN_A, 500);
    pwm_set_chan_level(slice_num_ch6, PWM_CHAN_B, 600);
    pwm_set_chan_level(slice_num_ch7, PWM_CHAN_A, 700);
    pwm_set_chan_level(slice_num_ch8, PWM_CHAN_B, 800);
    pwm_set_chan_level(slice_num_ch9, PWM_CHAN_A, 900);
    pwm_set_chan_level(slice_num_ch10, PWM_CHAN_B, 1000);
    pwm_set_chan_level(slice_num_ch11, PWM_CHAN_A, 1100);
    pwm_set_chan_level(slice_num_ch12, PWM_CHAN_B, 1200);
    pwm_set_chan_level(slice_num_ch13, PWM_CHAN_A, 1300);
    pwm_set_chan_level(slice_num_ch14, PWM_CHAN_B, 1400);
    pwm_set_chan_level(slice_num_ch15, PWM_CHAN_A, 1500);
    pwm_set_chan_level(slice_num_ch16, PWM_CHAN_B, 1580);

    pwm_set_enabled(slice_num_ch1, true);
    pwm_set_enabled(slice_num_ch2, true);
    pwm_set_enabled(slice_num_ch3, true);
    pwm_set_enabled(slice_num_ch4, true);
    pwm_set_enabled(slice_num_ch5, true);
    pwm_set_enabled(slice_num_ch6, true);
    pwm_set_enabled(slice_num_ch7, true);
    pwm_set_enabled(slice_num_ch8, true);
    pwm_set_enabled(slice_num_ch9, true);
    pwm_set_enabled(slice_num_ch10, true);
    pwm_set_enabled(slice_num_ch11, true);
    pwm_set_enabled(slice_num_ch12, true);
    pwm_set_enabled(slice_num_ch13, true);
    pwm_set_enabled(slice_num_ch14, true);
    pwm_set_enabled(slice_num_ch15, true);
    pwm_set_enabled(slice_num_ch16, true);
    

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

        level = (level + 1) % 256;
        sleep_ms(10);


        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);

        // gpio_put(LED_PIN, 1);
        // sleep_ms(25);
        // gpio_put(LED_PIN, 0);
        // sleep_ms(25);
    }

}
