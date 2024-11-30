#include "spi.h"

void SPI_init(spi_device_handle_t* spi)
{
    spi_bus_config_t bus = {
        .mosi_io_num = MOSI,                    //< mosi pin
        .miso_io_num = -1,                      //< miso pin
        .sclk_io_num = CLK,                     //< clk pin
        .quadwp_io_num = -1,                    // Write Protect dla Quad mode
        .quadhd_io_num = -1,                    // Hold signal dla Quad mode
        .max_transfer_sz = MAX_DATA_SIZE_BYTES  // Maximum transfer size in bytes (dalem 4 * 2 bo 4 wyswietlacze po rejestr+dane(czyli 1 + 1 bajtow))
    };
    spi_device_interface_config_t dev = {
        //.command_bits = 0,
        //.address_bits = 0,
        //.dummy_bits = 0,
        .mode = 0,                      // 0 = when idle clock low data changes on falling edge, 1 = rising edge, 3 = high changing on rising edge, 4 = high changing on falling edge
        //.clock_source = SPI_CLK_SRC_DEFAULT,
        //.duty_cycle_pos = 0,            // Duty cycle of positive clock, in 1/256th increments (128 = 50%/50% duty). Setting this to 0 (=not setting it) is equivalent to setting this to 128.
        //.cs_ena_pretrans = 0,           // Amount of SPI bit-cycles the cs should be activated before the transmission (0-16). This only works on half-duplex transactions.
        //.cs_ena_posttrans = 0,          // Amount of SPI bit-cycles the cs should stay active after the transmission (0-16)
        .clock_speed_hz = 5000000,        // 1 MHz
        //.input_delay_ns = 0,            // Maximum data valid time of slave. The time required between SCLK and MISO valid, including the possible clock delay from slave to master. The driver uses this value to give an extra delay before the MISO is ready on the line.
        .spics_io_num = CS,               // wybranie pinu cs
        .queue_size = 1,                  // ?? Transaction queue size. This sets how many transactions can be 'in the air' (queued using spi_device_queue_trans but not yet finished using spi_device_get_trans_result) at the same time.
    };

    esp_err_t ret;
#ifdef WROOM
    ret = spi_bus_initialize(VSPI_HOST, &bus, SPI_DMA_CH_AUTO); ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(VSPI_HOST, &dev, spi); ESP_ERROR_CHECK(ret);
#endif
#ifdef MINI
    ret = spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO); ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(SPI2_HOST, &dev, spi); ESP_ERROR_CHECK(ret);
#endif

}