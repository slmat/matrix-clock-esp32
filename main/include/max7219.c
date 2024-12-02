#include "max7219.h"

spi_device_handle_t SPi;

esp_err_t SPI_init()
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
        .mode = 0,                        // 0 = zegar normalnie w stanie niskim i zmiana na opadajacym zboczu, 1 = zegar normalnie w stanie niskim i zmiana na wzrastajacym zboczu, 3 = zegar normalnie w stanie wysokim i zmiana na wzrastajacym zboczu, 4 = zegar normalnie w stanie wysokim i zmiana na opadajacym zboczu,
        .clock_speed_hz = 5000000,        // 5 MHz (ten spi chyba ma max 10MHz a max7219 ma max 8MHz jesli dobrze pamietam)
        .spics_io_num = CS,               
        .queue_size = 1,                  // ile transakcji moze byc zakolejkowanych (nie skonczonych)
    };

    esp_err_t ret;
    
#ifdef WROOM
    ret = spi_bus_initialize(VSPI_HOST, &bus, SPI_DMA_CH_AUTO);
    if(ret) return ret;
    ret = spi_bus_add_device(VSPI_HOST, &dev, spi);
    if(ret) return ret;
#endif
#ifdef MINI
    ret = spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO);
    if(ret) return ret;
    ret = spi_bus_add_device(SPI2_HOST, &dev, &SPi);
    if(ret) return ret;
#endif
    return ret;

}

esp_err_t max7219_sendm(const uint8_t reg, const uint8_t data)
{
    uint8_t tx_data[MAX_COUNT*2] = {reg, data, reg, data, reg, data, reg, data};

    spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));

    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;

    return spi_device_transmit(SPi, &trans);
}

esp_err_t max7219_sendrow(uint8_t reg, const uint8_t * data)
{
    uint8_t tx_data[MAX_COUNT*2] = {reg, *(data+0), reg, *(data+1), reg, *(data+2), reg, *(data+3)};

    spi_transaction_t trans;

    memset(&trans, 0, sizeof(trans));
    trans.length = MAX_COUNT * 8 * 2;
    trans.tx_buffer = tx_data;
    trans.rx_buffer = NULL;
    trans.flags = 0;
    
    return spi_device_transmit(SPi, &trans);
}

esp_err_t max7219_displayTime(const uint8_t * time)
{
    uint8_t data[4];
    esp_err_t ret;
    for(uint8_t i = 0; i < 7; ++i)
    {
#ifdef SEPARATOR
        switch (i)
        {
        case(0):
        case(3):
        case(4):
            data[0] = *(time+i);
            data[1] = (*(time+8+i)<<1);
            data[2] =  (*(time+16+i)>>1);
            data[3] =  *(time+24+i);
        break;
        
        default:
            data[0] = *(time+i);
            data[1] = ((*(time+8+i)<<1) | 0x01);
            data[2] = ((*(time+16+i)>>1) | 0x80);
            data[3] =  *(time+24+i);
           
        break;
        }
#else
        data[0] = *(time+i);
        data[1] = (*(time+8+i)<<1);
        data[2] =  (*(time+16+i)>>1);
        data[3] =  *(time+24+i);
#endif

        ret = max7219_sendrow(0x01+i, data);
        if(ret) return ret;
    }

    return ret;
}

esp_err_t max7219_changeIntensity(const uint8_t b)
{
    if(b == 0)
    {
        return max7219_sendm(0x0C, 0x00);
    }
    else if(b == 1)
    {
        esp_err_t ret = max7219_sendm(0x0C, 0x01);
        if(ret) return ret;
    }

#ifdef DEBUG
    ESP_LOGI("MAX", "Brightness %d", b);
#endif

    return max7219_sendm(0x0A, b-1);
}

esp_err_t max7219_underline(uint8_t bits)
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
    return max7219_sendrow(0x08, data);
}

esp_err_t max7219_init()
{
    esp_err_t ret;
    ret = max7219_sendm(0x09, 0x00);    // decode mode wylaczony (bez BCD dla poszczegolnych bitow)
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "decode off");
#endif

    ret = max7219_sendm(0x0A, 0x01);    // ustawienie jasnosci na najnizsza wartosc 0xXF najwiecej
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "jasnosc");
#endif

    ret = max7219_sendm(0x0B, 0x07);    // scan limit czyli ile diod ma byc wykorzystywane my chcemy wsystkie czyli 0xX7
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "scan limit");
#endif

    ret = max7219_sendm(0x0C, 0x01);    // 1 - Normal mode (dziaua)  0 - Shutdown mode (nie dziaua)
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "shutdown");
#endif

#ifdef DISPLAY_TEST
    ret = max7219_sendm(0x0F, 0x01);    // display test na odwrut niz shutdown mode jak jest 1 - robi test czy sie swieca wszystkie lampki
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "display test");
#endif

#else
    ret = max7219_sendm(0x0F, 0x00);
    if(ret) return ret;

#ifdef DEBUG
    ESP_LOGI("MAX", "no display test");
#endif

#endif

    return ret;
}

esp_err_t max7219_clear()
{
    esp_err_t ret;
    for(uint8_t i = 0; i < ROWS; ++i)
    {
        ret = max7219_sendm(0x01+i, 0x00);
        if(ret) return ret;
    }

#ifdef DEBUG
        ESP_LOGI("MAX", "Clear");
#endif

    return ret;
}


