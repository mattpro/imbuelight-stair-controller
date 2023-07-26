#include <stdint.h>
#include "hardware/i2c.h"
#include "config.h"

#include "pico/stdlib.h"

#define i2c_default PICO_DEFAULT_I2C_INSTANCE


#define SSD1306_WRITE_MODE         _u(0xFE)
#define SSD1306_READ_MODE          _u(0xFF)

void I2C_init(void)
{
    i2c_init(i2c_default, 400000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

sleep_ms(2500);

    int ret;
    uint8_t rxdata;
    printf("I2C SCAN: \r\n"); 
    for (int i = 0 ; i < 128 ; i ++ )
    {
        ret = i2c_read_blocking(i2c_default, i, &rxdata, 1, false);
        if ( ret > 0 )  printf("Found i2c dev: %02x\r\n", i); 
        sleep_ms(25);
    }


}


void I2C_write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    //i2c_write_blocking(i2c_default, (DevAddress & SSD1306_WRITE_MODE), buf, 2, false);

}


void I2C_read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{

}



uint8_t read_io_expander_state(void)
{
    int ret;
    uint8_t data = 0;
    ret = i2c_write_blocking(i2c_default, 0x18, &data, 1, false);
    ret = i2c_read_blocking(i2c_default, 0x18, &data, 1, false);
    return data;
}
