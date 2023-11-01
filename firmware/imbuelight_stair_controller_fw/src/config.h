#ifndef __CONFIG_H
#define __CONFIG_H

#include "rtt/RTT/SEGGER_RTT.h"

// SETTINGS
#define GAMMA_FACTOR 4.0f

// ENUM
typedef enum {
    DIR_DOWN_TO_UP,
    DIR_UP_TO_DOWN
} effect_dir_t;

typedef enum {
    TURN_ON,
    TURN_OFF
} turn_dir_t;


// PINOUT
// Standard PWM
#define PWM_CHANNEL_1_PIN       0   // Slice: 0 CH: A
#define PWM_CHANNEL_2_PIN       1   // Slice: 0 CH: B
#define PWM_CHANNEL_10_PIN      18   // Slice: 1 CH: A    SLICE 1A moze byc albo pin 2 lub 18 // kanał 24 trzeba usunąć - pinu 2 nie moge użyć
#define PWM_CHANNEL_23_PIN      3   // Slice: 1 CH: B  
#define PWM_CHANNEL_3_PIN       4   // Slice: 2 CH: A
#define PWM_CHANNEL_4_PIN       5   // Slice: 2 CH: B
#define PWM_CHANNEL_22_PIN      6   // Slice: 3 CH: A
#define PWM_CHANNEL_21_PIN      7   // Slice: 3 CH: B
#define PWM_CHANNEL_20_PIN      8   // Slice: 4 CH: A
#define PWM_CHANNEL_19_PIN      9   // Slice: 4 CH: B`
#define PWM_CHANNEL_18_PIN      10  // Slice: 5 CH: A
#define PWM_CHANNEL_17_PIN      11  // Slice: 5 CH: B
#define PWM_CHANNEL_16_PIN      12  // Slice: 6 CH: A
#define PWM_CHANNEL_15_PIN      13  // Slice: 6 CH: B
#define PWM_CHANNEL_14_PIN      14  // Slice: 7 CH: A
#define PWM_CHANNEL_13_PIN      15  // Slice: 7 CH: B

// PWM from PIO 0
#define PIO_CHANNEL_5_8       pio0
#define PWM_CHANNEL_5_PIN       28 
#define PWM_CHANNEL_6_PIN       27 
#define PWM_CHANNEL_7_PIN       26 
#define PWM_CHANNEL_8_PIN       22 

// PWM from PIO 1
#define PIO_CHANNEL_9_12       pio1
#define PWM_CHANNEL_9_PIN       19 
#define PWM_CHANNEL_11_PIN      17
#define PWM_CHANNEL_12_PIN      16 

// I2C pin
#define I2C_SDA_PIN             20
#define I2C_SCL_PIN             21

#endif