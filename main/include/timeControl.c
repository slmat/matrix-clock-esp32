#include "timeControl.h"


esp_err_t clock_init(Clock *a)
{
    esp_err_t ret;

    for(uint8_t i = 0; i < 4; ++i)
    {
        a->time[i] = i;
        clock_update_timeChar(i, a);
    }

    a->intensity = 0x02;

    ret = SPI_init();
    if(ret) return ret;

    ret = max7219_init();
    if(ret) return ret;

    ret = max7219_clear();
    if(ret) return ret;

    a->ready = true;

#ifdef DEBUG
        ESP_LOGI("Clock", "zainicjalizowany");
#endif
    return ret;
}

bool clock_inc_intensity(Clock *a)
{
    if(a->intensity < 0x0F)
    {
        a->intensity += 1;
    }
    else
    {
        return true;
    }
    return false;
}

bool clock_dec_intensity(Clock *a)
{
    if(a->intensity > 0x00)
    {
        a->intensity -= 1;
    }
    else 
    {
        return true;
    }
    return false;
}

void clock_set_intensity(Clock *a,const uint8_t i)
{
    return;
}

void clock_update_timeChar(const uint8_t place, Clock *a)
{
    uint8_t digit = a->time[place];
    for(uint8_t i = 0; i < 8; ++i)
    {
        a->timeChars[place][i] = digits[digit][i];
    }
}

bool clock_add_minute(Clock *a)
{
    if(a->time[3] >= 9)
    {
        a->time[3] = 0;  
        clock_update_timeChar(3, a);

        if(a->time[2] >= 5)
        {
            a->time[2] = 0;
            clock_update_timeChar(2, a);
            return true;
        }
        else
        {
            a->time[2] += 1;
            clock_update_timeChar(2, a);
        }
    }
    else
    {
        a->time[3] += 1;
        clock_update_timeChar(3, a);
    }
    return false;
}

bool clock_add_hour(Clock *a)
{
    if(a->time[0] <= 1 && a->time[1] >= 9)
    {
        a->time[1] = 0;
        clock_update_timeChar(1, a);
        a->time[0] += 1;
        clock_update_timeChar(0, a);
    }
    else if(a->time[0] >= 2 && a->time[1] >= 3)
    {
        a->time[1] = 0;
        clock_update_timeChar(1, a);
        a->time[0] = 0;
        clock_update_timeChar(0, a);
        return true;
    }
    else
    {
        a->time[1] += 1;
        clock_update_timeChar(1, a);
    }
    return false;
}

void clock_sub_minute(Clock *a)
{
    if(a->time[2] == 0 && a->time[3] == 0)
    {
        a->time[2] = 5;
        clock_update_timeChar(2, a);
        a->time[3] = 9;
        clock_update_timeChar(3, a);
    }
    else
    {
        if(a->time[3] == 0)
        {
            a->time[2] -= 1;
            clock_update_timeChar(2, a);
            a->time[3] = 9;
            clock_update_timeChar(3, a);
        }
        else
        {
            a->time[3] -= 1;
            clock_update_timeChar(3, a);
        }
    }
}

void clock_sub_hour(Clock *a)
{
    if(a->time[0] == 0 && a->time[1] == 0)
    {
        a->time[0] = 2;
        clock_update_timeChar(2, a);
        a->time[1] = 3;
        clock_update_timeChar(1, a);
    }
    else
    {
        if(a->time[1] == 0)
        {
            a->time[1] = 9;
            clock_update_timeChar(1, a);
            a->time[0] -= 1;
            clock_update_timeChar(0, a);
        }
        else
        {
            a->time[1] -= 1;
            clock_update_timeChar(1, a);
        }
    }
}

void clock_update(Clock *a)
{
    if(clock_add_minute(a))
    {
        clock_add_hour(a);
    }
}

void CLock_loop(void *t)
{/*
    Clock *a = (Clock *)t;
    TickType_t last = xTaskGetTickCount();
    while(1)
    {
        if(a->ready == 0)
        {
            max7219_displayTime((uint8_t *)a->timeChars);
            count(a);
            vTaskDelayUntil(&last, pdMS_TO_TICKS(1000));
        }
        else
        {
            if(a->ready == true)
            {
                last = xTaskGetTickCount();
                a->ready = 0;
            }
            else
            {
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        }
    }
*/}

void dummy(Clock *a)
{
    TickType_t last = xTaskGetTickCount();
    while(1)
    {
        BLOCK_UNTIL(
            ESP_ERROR_CHECK(max7219_displayTime((uint8_t *)a->timeChars));
            clock_update(a);
            ,a->ready
        );

#ifdef DEBUG
        ESP_LOGI("Clock", "%d%d:%d%d", a->time[0], a->time[1], a->time[2],a->time[3]);
#endif
        vTaskDelayUntil(&last, pdMS_TO_TICKS(60000));
    }
    
}