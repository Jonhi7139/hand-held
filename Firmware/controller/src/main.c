#include "buttons.h"
#include "sticks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    xTaskCreate(buttons_task, "buttons", 4096, NULL, 5, NULL);
    xTaskCreate(sticks_task, "sticks", 4096, NULL, 5, NULL);
}
