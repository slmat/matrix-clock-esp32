#ifndef SPIH
#define SPIH

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "defines.h"

/**
 * 
 *  stara wersja programu zawierala 
 *  wykorzystanie tego pliku 
 *  nastepna tez bedzie zawierac ten plik
 *  ale dopiero po refaktoryzacji kodu
 *  aby byl bardziej przejrzysty
 * 
 * 
 * 
 */

static spi_device_handle_t spi;


void SPI_init();

#endif