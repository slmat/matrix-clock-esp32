#ifndef MAX7219H
#define MAX7219H
#include "esp_log.h"
#include <driver/spi_master.h>
#include <string.h>
#include "defines.h"

extern spi_device_handle_t SPi;

esp_err_t SPI_init();
esp_err_t max7219_sendm(const uint8_t reg, const uint8_t data);
esp_err_t max7219_sendrow(const uint8_t reg, const uint8_t * data);
esp_err_t max7219_displayTime(const uint8_t *time);
esp_err_t max7219_changeIntensity(const uint8_t b);
esp_err_t max7219_underline(uint8_t bits);
esp_err_t max7219_init();
esp_err_t max7219_clear();


#endif