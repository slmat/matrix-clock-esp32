#ifndef BUTTONS_H
#define BUTTONS_H

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "defines.h"
#include "timeControl.h"

static enum State {EDIT_MODE_NONE, EDIT_MODE_HOURS, EDIT_MODE_MINUTES};

void buttons_init();
void buttons_updateLoop(Clock *a, TaskHandle_t *t);

#endif