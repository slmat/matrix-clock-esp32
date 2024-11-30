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
// spi_device_handle_t spi;
TaskHandle_t timeHandle;
uint8_t ass[2];    

void app_main(void)
{
    initT(&t/*, &spi*/);
    xTaskCreatePinnedToCore(dummy, "time utils", DISPLAY_STACK, &t, 1, &timeHandle, 0);
    buttons_init();
    buttons_updateLoop(&t, &timeHandle);
}