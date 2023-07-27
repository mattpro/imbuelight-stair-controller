#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>

void I2C_init(void);
void I2C_write(uint8_t dev_add, uint8_t mem_add, uint8_t *pData, uint16_t data_size);
void I2C_read(uint16_t dev_add, uint16_t mem_add, uint8_t *pData, uint16_t data_size);

#endif
