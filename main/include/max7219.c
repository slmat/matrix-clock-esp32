#include "max7219.h"

spi_device_handle_t SPi;

void SPI_init(/*spi_device_handle_t *spi*/)
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
    ret = spi_bus_add_device(SPI2_HOST, &dev, &SPi); ESP_ERROR_CHECK(ret);
#endif

}

void max7219_send8(/*spi_device_handle_t spi,*/uint8_t reg, uint8_t data)
{
    esp_err_t ret;
    uint8_t tx_data[2] = {reg, data};

    spi_transaction_t trans;
    memset(&trans, 0, sizeof(trans));

    trans.length = 16;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;
    //spi_device_acquire_bus(SPi, portMAX_DELAY);
    ret = spi_device_transmit(SPi, &trans); ESP_ERROR_CHECK(ret);
    //spi_device_release_bus(SPi);
}

void max7219_sendm(/*spi_device_handle_t spi,*/ const uint8_t reg, const uint8_t data)
{
    esp_err_t ret;
    uint8_t tx_data[MAX_COUNT*2] = {reg, data, reg, data, reg, data, reg, data};

    spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));
    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;
    //spi_device_acquire_bus(SPi, portMAX_DELAY);
    ret = spi_device_transmit(SPi, &trans); ESP_ERROR_CHECK(ret);
    //spi_device_release_bus(spi);
}

void max7219_sendrow(/*spi_device_handle_t spi,*/ uint8_t reg, const uint8_t * data)
{
    esp_err_t ret;
    uint8_t tx_data[MAX_COUNT*2] = {reg, *(data+0), reg, *(data+1), reg, *(data+2), reg, *(data+3)};

    spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));
    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;
    
    ret = spi_device_transmit(SPi, &trans); ESP_ERROR_CHECK(ret);
}

void max7219_displayTime(/*spi_device_handle_t spi,*/ const uint8_t * time)
{
    uint8_t data[4];
    //spi_device_acquire_bus(spi, portMAX_DELAY);
    for(uint8_t i = 0; i < 7; ++i)
    {
        switch (i)
        {
        case(0):
        case(3):
        case(4):
        /*case(7):*/
            data[0] = *(time+i);
            data[1] = (*(time+8+i)<<1);
            data[2] =  (*(time+16+i)>>1);
            data[3] =  *(time+24+i);
            max7219_sendrow(/*spi,*/ 0x01+i, data);
        break;
        
        default:
            data[0] = *(time+i);
            data[1] = ((*(time+8+i)<<1) | 0x01);
            data[2] = ((*(time+16+i)>>1) | 0x80);
            data[3] =  *(time+24+i);
            max7219_sendrow(/*spi,*/ 0x01+i, data);
        break;
        }
    }
    //spi_device_release_bus(spi);
}

void max7219_changeIntensity(/*spi_device_handle_t spi,*/ const uint8_t b)
{
    if(b == 0)
    {
        max7219_sendm(/*spi,*/ 0x0C, 0x00);
        return;
    }
    else if(b == 1)
    {
        max7219_sendm(/*spi,*/ 0x0C, 0x01);
    }
    #ifdef DEBUG
    ESP_LOGI("MAX", "Brightness %d", b);
    #endif
    max7219_sendm(/*spi,*/ 0x0A, b-1);
}

void max7219_underline(/*spi_device_handle_t spi,*/ uint8_t bits)
{
    uint8_t data[4];
    switch(bits)
    {
        case(1):
            data[0] = 0x7f;
            data[1] = 0xfe;
            data[2] = 0;
            data[3] = 0;
        break;
        case(2):
            data[0] = 0;
            data[1] = 0;
            data[2] = 0x7f;
            data[3] = 0xfe;
        break;
        default:
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
        break;
    }
    max7219_sendrow(/*spi,*/ 0x08, data);
}

void max7219_init(/*spi_device_handle_t spi*/)
{
    max7219_sendm(/*spi,*/ 0x09, 0x00);    // decode mode wylaczony (bez BCD dla poszczegolnych bitow)
    #ifdef DEBUG
    ESP_LOGI("MAX", "decode");
    #endif
    max7219_sendm(/*spi,*/ 0x0A, 0x01);    // ustawienie jasnosci na najnizsza wartosc 0xXF najwiecej
    #ifdef DEBUG
    ESP_LOGI("MAX", "jasnosc");
    #endif
    max7219_sendm(/*spi,*/ 0x0B, 0x07);    // scan limit czyli ile diod ma byc wykorzystywane my chcemy wsystkie czyli 0xX7
    #ifdef DEBUG
    ESP_LOGI("MAX", "scan limit");
    #endif
    max7219_sendm(/*spi,*/ 0x0C, 0x01);    // 1 - Normal mode (dziaua)  0 - Shutdown mode (nie dziaua)
    #ifdef DEBUG
    ESP_LOGI("MAX", "shutdown");
    #endif
    #ifdef DISPLAY_TEST
        max7219_sendm(/*spi,*/ 0x0F, 0x01);    // display test na odwrut niz shutdown mode jak jest 1 - robi test czy sie swieca wszystkie lampki
        #ifdef DEBUG
        ESP_LOGI("MAX", "display test");
        #endif
    #else
        max7219_sendm(/*spi,*/ 0x0F, 0x00);
        #ifdef DEBUG
        ESP_LOGI("MAX", "no display test");
        #endif
    #endif
}

void max7219_clear(/*spi_device_handle_t spi*/)
{
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        max7219_sendm(/*spi,*/ 0x01+i, 0x00);
    }
    #ifdef DEBUG
        ESP_LOGI("MAX", "Clear");
    #endif
}


