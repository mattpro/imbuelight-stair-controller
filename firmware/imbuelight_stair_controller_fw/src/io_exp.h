#ifndef _IO_EXP_H_
#define _IO_EXP_H_

#include <stdint.h>

#define IO_EXP_I2C_ADDRESS  0x18

#define BUTTON_1            0x02
#define BUTTON_2            0x01
#define BUTTON_3            0x10
#define BUTTON_4            0x08
#define SENS_TOP            0x04
#define SENS_BOTTOM         0x20
#define SWITCH_1            0x80
#define SWITCH_2            0x40


void IO_EXP_init(void);
void IO_EXP_pooling(void);
void IO_EXP_reg_event_sens_top_cbfunc(void(*sens_top_enter)(void), void(*sens_top_exit)(void));
void IO_EXP_reg_event_sens_bottom_cbfunc(void(*sens_bottom_enter)(void), void(*sens_bottom_exit)(void));

#endif