#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>


void I2C_init(void);
void I2C_write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
void I2C_read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)


#endif