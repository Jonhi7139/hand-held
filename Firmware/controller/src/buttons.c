#include "buttons.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Deifines//
#define PIN_UP 32
#define PIN_LEFT 13
#define PIN_RIGHT 33
#define PIN_DOWN 14

#define BTN_PRESSED 0
#define BTN_RELEASED 1

//Declare private functions
static void config(struct button *b, int count);

static void read_buttons(struct button *b, int count);


//Components initialization
static struct button buttons[]=
{
    {.pin = PIN_UP, .state_now = BTN_RELEASED, .last_state = BTN_RELEASED, .name = "up"},
    {.pin = PIN_DOWN, .state_now = BTN_RELEASED, .last_state = BTN_RELEASED, .name = "down"},
    {.pin = PIN_LEFT, .state_now = BTN_RELEASED, .last_state = BTN_RELEASED, .name = "left"},
    {.pin = PIN_RIGHT, .state_now = BTN_RELEASED, .last_state = BTN_RELEASED, .name = "right"},   
};
static int Num_Buttons = (sizeof(buttons)/sizeof(buttons[0]));

//write functions
static void config (struct button *b, int count)
{
    for (int i = 0;i<count;i++)
    {
        gpio_reset_pin(b[i].pin);
        gpio_set_direction(b[i].pin,GPIO_MODE_INPUT);
        gpio_set_pull_mode(b[i].pin, GPIO_PULLUP_ONLY);
    }
}

static void read_buttons(struct button *b, int count)
{
    for (int i = 0;i<count;i++)
    {
        b[i].state_now = gpio_get_level(b[i].pin);
        
        if(b[i].state_now!=b[i].last_state)
        {   
            if(b[i].state_now == BTN_PRESSED)
            {
                printf("%s pressed\n",b[i].name);
            }
            else
            {
                printf("%s released\n",b[i].name);
            }
            b[i].last_state=b[i].state_now;
        }
    } 
}

void buttons_task(void *parameters)
{
    config(buttons, Num_Buttons);
    TickType_t ticks = xTaskGetTickCount();
    while(1)
    {
        read_buttons(buttons, Num_Buttons);
        vTaskDelayUntil(&ticks, pdMS_TO_TICKS(10));
    }
}


