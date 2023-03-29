
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pwm.pio.h"


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
    const uint LED_PIN = 2;
    const uint PWM_TEST_PIN = 3;


    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &pwm_program);
    pwm_program_init(pio, sm, offset, PWM_TEST_PIN);
    pio_pwm_set_period(pio, sm, (1u << 16) - 1);
    int level = 0;

    while (true) 
    {

        pio_pwm_set_level(pio, sm, level * level);
        level = (level + 1) % 256;
        sleep_ms(10);

        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);
    }

}
