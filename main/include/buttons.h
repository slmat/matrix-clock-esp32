#ifndef BUTTONS_H
#define BUTTONS_H

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "defines.h"
#include "timeControl.h"

#define BUTTON_CLICKED(x) gpio_get_level(x) == 0
#define BUTTON_NOTCLICKED(x) gpio_get_level(x) == 1

static enum State {EDIT_MODE_NONE, EDIT_MODE_HOURS, EDIT_MODE_MINUTES};

void buttons_updateLoop(Clock *a, TaskHandle_t *t);
esp_err_t buttons_init();

#endif