#ifndef __CONFIG_H
#define __CONFIG_H

#define PIO_STATE_MACHINE_1   0
#define PIO_STATE_MACHINE_2   1
#define PIO_STATE_MACHINE_3   2
#define PIO_STATE_MACHINE_4   3


// PINOUT
// Standard PWM
#define PWM_CHANNEL_1_PIN       0
#define PWM_CHANNEL_2_PIN       1
#define PWM_CHANNEL_3_PIN       4
#define PWM_CHANNEL_4_PIN       5
#define PWM_CHANNEL_5_PIN       28
#define PWM_CHANNEL_6_PIN       27
#define PWM_CHANNEL_7_PIN       26
#define PWM_CHANNEL_8_PIN       22
#define PWM_CHANNEL_9_PIN       19
#define PWM_CHANNEL_10_PIN      18
#define PWM_CHANNEL_11_PIN      17
#define PWM_CHANNEL_12_PIN      16
#define PWM_CHANNEL_13_PIN      15
#define PWM_CHANNEL_14_PIN      14     
#define PWM_CHANNEL_15_PIN      13
#define PWM_CHANNEL_16_PIN      12

// PWM from PIO 1
#define PIO_CHANNEL_17_20       pio0
#define PWM_CHANNEL_17_PIN      11
#define PWM_CHANNEL_18_PIN      10
#define PWM_CHANNEL_19_PIN      9
#define PWM_CHANNEL_20_PIN      8

// PWM from PIO 2
#define PIO_CHANNEL_21_24       pio1
#define PWM_CHANNEL_21_PIN      7
#define PWM_CHANNEL_24_PIN      2
#define PWM_CHANNEL_23_PIN      3
#define PWM_CHANNEL_22_PIN      6

// I2C pin
#define I2C_SDA_PIN             20
#define I2C_SCL_PIN             21

#endif