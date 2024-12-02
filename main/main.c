#include <stdio.h>
#include "esp_log.h"
#include "esp_heap_caps.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_attr.h>

#include "include/timeControl.h"
#include "include/max7219.h"
#include "include/buttons.h"

Clock t;
TaskHandle_t clockHandle;    

void app_main(void)
{
    ESP_ERROR_CHECK(clock_init(&t));
    ESP_ERROR_CHECK(buttons_init());

    xTaskCreatePinnedToCore(dummy, "Clock", DISPLAY_STACK, &t, CLOCK_PRIORITY, &clockHandle, CORE);

    buttons_update(&t, &clockHandle);
}