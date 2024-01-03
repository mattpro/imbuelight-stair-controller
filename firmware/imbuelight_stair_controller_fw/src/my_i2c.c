#include <stdint.h>
#include "hardware/i2c.h"
#include "config.h"

#include "pico/stdlib.h"

#define i2c_default PICO_DEFAULT_I2C_INSTANCE

static uint8_t i2c_buffer[100];


void I2C_init(void)
{
    i2c_init(i2c_default, 400000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);


#if 0
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

#endif
}


void I2C_write(uint8_t dev_add, uint8_t mem_add, uint8_t *pData, uint16_t data_size)
{
    int ret = 0;
    i2c_buffer[0] = mem_add;
    memcpy(pData, &i2c_buffer[1], data_size);

    ret = i2c_write_blocking(i2c_default, dev_add, i2c_buffer, data_size + 1, false);
    if ( ret < 0 )
    {
        printf("i2c write error");
    }
}


void I2C_read(uint16_t dev_add, uint16_t mem_add, uint8_t *pData, uint16_t data_size)
{
    int ret;

    ret = i2c_write_blocking(i2c_default, dev_add, &mem_add, 1, false);
    if ( ret < 0 )
    {
        printf("i2c write error");
    }
    ret = i2c_read_blocking(i2c_default, dev_add, pData, 1, false);
    if ( ret < 0 )
    {
        printf("i2c write error");
    }
}

