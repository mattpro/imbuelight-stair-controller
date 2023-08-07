#include <stdint.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "my_i2c.h"
#include "config.h"
#include "io_exp.h"

static uint8_t new_io_exp_state;
static uint8_t old_io_exp_state;
static uint8_t change_io_exp_state;

// struct with function pointer
typedef struct io_expander_evet
{    
    void (*_sens_top_enter) (void); 
    void (*_sens_top_exit) (void);
    void (*_sens_bottom_enter) (void); 
    void (*_sens_bottom_exit) (void);        

    // TODO:
    void (*_switch1_press)  (void);       
    void (*_switch1_unpress)  (void);   
    void (*_switch2_press)  (void);       
    void (*_switch2_unpress)  (void);   
    void (*_button1_press)  (void);       
    void (*_button1_unpress)  (void);   
    void (*_button2_press)  (void);       
    void (*_button2_unpress)  (void);   
    void (*_button3_press)  (void);       
    void (*_button3_unpress)  (void);   
    void (*_button4_press)  (void);       
    void (*_button4_unpress)  (void);   
}io_expander_event_t;


// default function 
void default_sens_top_enter(void)       {printf("Top enter\r\n");}
void default_sens_top_exit(void)        {printf("Top exit\r\n");}
void default_sens_bottom_enter(void)    {printf("Bottom enter\r\n");}
void default_sens_bottom_exit(void)     {printf("Bottom exit\r\n");}

void default_sens_switch1_press(void)   {printf("Switch 1 press\r\n");}
void default_sens_switch1_unpress(void) {printf("Switch 1 unpress\r\n");}
void default_sens_switch2_press(void)   {printf("Switch 2 press\r\n");}
void default_sens_switch2_unpress(void) {printf("Switch 2 unpress\r\n");}

void default_sens_button1_press(void)   {printf("Button 1 press\r\n");}
void default_sens_button1_unpress(void) {printf("Button 1 unpress\r\n");}
void default_sens_button2_press(void)   {printf("Button 2 press\r\n");}
void default_sens_button2_unpress(void) {printf("Button 2 unpress\r\n");}
void default_sens_button3_press(void)   {printf("Button 3 press\r\n");}
void default_sens_button3_unpress(void) {printf("Button 3 unpress\r\n");}
void default_sens_button4_press(void)   {printf("Button 4 press\r\n");}
void default_sens_button4_unpress(void) {printf("Button 4 unpress\r\n");}

// define and set default function
io_expander_event_t io_expander_event =
{
    default_sens_top_enter,
    default_sens_top_exit,
    default_sens_bottom_enter,
    default_sens_bottom_exit,

    default_sens_switch1_press,
    default_sens_switch1_unpress,
    default_sens_switch2_press,
    default_sens_switch2_unpress,

    default_sens_button1_press,
    default_sens_button1_unpress,
    default_sens_button2_press,
    default_sens_button2_unpress,
    default_sens_button3_press,
    default_sens_button3_unpress,
    default_sens_button4_press,
    default_sens_button4_unpress,
};


void IO_EXP_reg_event_sens_top_cbfunc(void(*sens_top_enter)(void), void(*sens_top_exit)(void))
{
    if(sens_top_enter != NULL )
    {
        io_expander_event._sens_top_enter = sens_top_enter;
    }
    else
    {
        io_expander_event._sens_top_enter = default_sens_top_enter;
    }

    if(sens_top_exit != NULL )
    {
        io_expander_event._sens_top_exit = sens_top_exit;
    }
    else
    {
        io_expander_event._sens_top_exit = default_sens_top_exit;
    }
}


void IO_EXP_reg_event_sens_bottom_cbfunc(void(*sens_bottom_enter)(void), void(*sens_bottom_exit)(void))
{
    if(sens_bottom_enter != NULL )
    {
        io_expander_event._sens_bottom_enter = sens_bottom_enter;
    }
    else
    {
        io_expander_event._sens_bottom_enter = default_sens_bottom_enter;
    }

    if(sens_bottom_exit != NULL )
    {
        io_expander_event._sens_bottom_exit = sens_bottom_exit;
    }
    else
    {
        io_expander_event._sens_bottom_exit = default_sens_bottom_exit;
    }
}


void IO_EXP_init(void)
{
    uint8_t data = 0;
    I2C_read(IO_EXP_I2C_ADDRESS, 0x00, &data, 1);
    new_io_exp_state = data;
    old_io_exp_state = data;
}


void IO_EXP_pooling(void)
{
    I2C_read(IO_EXP_I2C_ADDRESS, 0x00, &new_io_exp_state, 1); 
    change_io_exp_state = new_io_exp_state ^ old_io_exp_state;
    //printf("Change: %02x\r\n", change_io_exp_state);

    switch( change_io_exp_state )
    {
        case BUTTON_1:
            new_io_exp_state & BUTTON_1 ? io_expander_event._button1_unpress() : io_expander_event._button1_press();
        break;
        case BUTTON_2:
            new_io_exp_state & BUTTON_2 ? io_expander_event._button2_unpress() : io_expander_event._button2_press();
        break;
        case BUTTON_3:
            new_io_exp_state & BUTTON_3 ? io_expander_event._button3_unpress() : io_expander_event._button3_press();
        break;
        case BUTTON_4:
            new_io_exp_state & BUTTON_4 ? io_expander_event._button4_unpress() : io_expander_event._button4_press();
        break;
        case SWITCH_1:
            new_io_exp_state & SWITCH_1 ? io_expander_event._switch1_unpress() : io_expander_event._switch1_press();
        break;
        case SWITCH_2:
            new_io_exp_state & SWITCH_2 ? io_expander_event._switch2_unpress() : io_expander_event._switch2_press();
        break;
        case SENS_TOP:
            new_io_exp_state & SENS_TOP ? io_expander_event._sens_top_exit() : io_expander_event._sens_top_enter();
        break;
        case SENS_BOTTOM:
            new_io_exp_state & SENS_BOTTOM ? io_expander_event._sens_bottom_exit() : io_expander_event._sens_bottom_enter();
        break;
    }

    old_io_exp_state = new_io_exp_state;
}
