#include "sticks.h"
#include "board.h"
#include <stdio.h>
#include "esp_adc/adc_oneshot.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

struct stick
{
    int x_pin;
    int y_pin;

    adc_channel_t x_ch;
    adc_channel_t y_ch;

    int x_read;
    int y_read;
    
    const char *name;


};

static void config(struct stick *s, int count);

static void read_sticks(struct stick *s, int count);

static struct stick sticks[]=
{
    {.name="Left Controller", .x_pin=PIN_X1, .y_pin=PIN_Y1,}
};

static int Num_sticks = (sizeof(sticks)/sizeof(sticks[0]));

static adc_oneshot_unit_handle_t adc1_handle;



static void config(struct stick *s, int count)
{
    adc_oneshot_unit_init_cfg_t unit_cfg = { .unit_id = ADC_UNIT_1 };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc1_handle));

    adc_oneshot_chan_cfg_t chan_cfg = 
    {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    for(int i = 0;i<count;i++)
    {
        adc_unit_t unit;
        adc_channel_t channel;
        ESP_ERROR_CHECK(adc_oneshot_io_to_channel(s[i].x_pin, &unit, &channel));
        s[i].x_ch=channel;
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, s[i].x_ch, &chan_cfg));

        ESP_ERROR_CHECK(adc_oneshot_io_to_channel(s[i].y_pin, &unit, &channel));
        s[i].y_ch=channel;
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, s[i].y_ch, &chan_cfg));

    }
}

static void read_sticks(struct stick *s, int count)
{
    for (int i = 0;i<count;i++)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, s[i].x_ch, &s[i].x_read));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, s[i].y_ch, &s[i].y_read));
        printf("%s: x=%d y=%d\n", s[i].name, s[i].x_read, s[i].y_read);
    }
}

void sticks_task(void *parameters)
{
    config(sticks, Num_sticks);
    TickType_t ticks = xTaskGetTickCount();
    while(1)
    {
        read_sticks(sticks, Num_sticks);
        vTaskDelayUntil(&ticks, pdMS_TO_TICKS(10));
    }


}