#ifndef MAX7219H
#define MAX7219H
#include "esp_log.h"
#include <driver/spi_master.h>
#include <string.h>
#include "defines.h"

extern spi_device_handle_t SPi;

void SPI_init();
void max7219_send8(/*spi_device_handle_t spi,*/ uint8_t reg, uint8_t data);
void max7219_sendm(/*spi_device_handle_t spi,*/ const uint8_t reg, const uint8_t data);
void max7219_sendrow(/*spi_device_handle_t spi,*/ const uint8_t reg, const uint8_t * data);
void max7219_displayTime(/*spi_device_handle_t spi,*/ const uint8_t *time);
void max7219_changeIntensity(/*spi_device_handle_t spi,*/ const uint8_t b);
void max7219_underline(/*spi_device_handle_t spi,*/uint8_t bits);
void max7219_init(/*spi_device_handle_t spi*/);
void max7219_clear();
//void startMax(void *a);


#endif