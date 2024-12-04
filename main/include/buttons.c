#include "buttons.h"


void buttons_update(Clock *a, TaskHandle_t *t) 
{
    TickType_t last_wake_time = xTaskGetTickCount();
    enum State editMode = EDIT_MODE_NONE;
    uint8_t state = 0;
    
    while(1)
    {
        if(editMode == EDIT_MODE_NONE)
        {
            BLOCK_UNTIL(
            switch(state)
            {
                case(0):
                if(BUTTON_CLICKED(BL)) state = 1;
                else if(BUTTON_CLICKED(BC)) state = 4;
                else if(BUTTON_CLICKED(BR)) state = 7; 
                break;

                case(1):
                    if(BUTTON_CLICKED(BL)) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(BUTTON_NOTCLICKED(BL)) state = 3;
                break;

                case(3):
                    if(BUTTON_NOTCLICKED(BL)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "JASNOSC LEWY ODKLIKNIETY");
#endif
                        clock_dec_intensity(a);
                        max7219_changeIntensity(a->intensity);

                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(BUTTON_CLICKED(BC)) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(BUTTON_NOTCLICKED(BC)) state = 6;
                break;

                case(6):
                    if(BUTTON_NOTCLICKED(BC)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "JASNOSC SRODKOWY ODKLIKNIETY"); 
#endif
#ifdef KILL_INSTEAD_OF_HALT
                        vTaskDelete(*(t));
#else
                        vTaskSuspend(*(t));
#endif
                        clock_sub_minute(a);
                        max7219_displayTime((uint8_t *)a->timeChars);
                        max7219_underline(1);

                        editMode = EDIT_MODE_HOURS;
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(BUTTON_CLICKED(BR)) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(BUTTON_NOTCLICKED(BR)) state = 9;
                break;  

                case(9):
                    if(BUTTON_NOTCLICKED(BR)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "JASNOSC PRAWY ODKLIKNIETY");
#endif
                        clock_inc_intensity(a);
                        max7219_changeIntensity(a->intensity); 

                        state = 0;
                    }
                    else state = 8;
                break;

                default:
                    state = 0;
                break;
            }
            ,a->ready);
        }
        else if(editMode == EDIT_MODE_HOURS)
        {
            switch(state)
            {
                case(0):
                if(BUTTON_CLICKED(BL)) state = 1;
                else if(BUTTON_CLICKED(BC)) state = 4;
                else if(BUTTON_CLICKED(BR)) state = 7; 
                break;

                case(1):
                    if(BUTTON_CLICKED(BL)) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(BUTTON_NOTCLICKED(BL)) state = 3;
                break;

                case(3):
                    if(BUTTON_NOTCLICKED(BL)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "GODZINY LEWY ODKLIKNIETY");
#endif
                        clock_sub_hour(a);
                        max7219_displayTime( (uint8_t *)a->timeChars); 
                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(BUTTON_CLICKED(BC)) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(BUTTON_NOTCLICKED(BC)) state = 6;
                break;

                case(6):
                    if(BUTTON_NOTCLICKED(BC)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "GODZINY SRODKOWY ODKLIKNIETY"); 
#endif
                        max7219_underline(2);
                        editMode = EDIT_MODE_MINUTES; 
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(BUTTON_CLICKED(BR)) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(BUTTON_NOTCLICKED(BR)) state = 9;
                break;  

                case(9):
                    if(BUTTON_NOTCLICKED(BR)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "GODZINY PRAWY ODKLIKNIETY");
#endif

                        clock_add_hour(a);
                        max7219_displayTime((uint8_t *)a->timeChars); 
                        state = 0;
                    }
                    else state = 8;
                break;

                default:
                    state = 0;
                break;
            }
        }
        else if(editMode == EDIT_MODE_MINUTES)
        {
            switch(state)
            {
                case(0):
                if(BUTTON_CLICKED(BL)) state = 1;
                else if(BUTTON_CLICKED(BC)) state = 4;
                else if(BUTTON_CLICKED(BR)) state = 7; 
                break;

                case(1):
                    if(BUTTON_CLICKED(BL)) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(BUTTON_NOTCLICKED(BL)) state = 3;
                break;

                case(3):
                    if(BUTTON_NOTCLICKED(BL)) 
                    {
#ifdef DEBUG
    ESP_LOGI("Button", "MINUTY LEWY ODKLIKNIETY");
#endif

                        clock_sub_minute(a); 
                        max7219_displayTime((uint8_t *)a->timeChars);
                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(BUTTON_CLICKED(BC)) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(BUTTON_NOTCLICKED(BC)) state = 6;
                break;

                case(6):
                    if(BUTTON_NOTCLICKED(BC)) 
                    {
                        max7219_underline(0);
#ifdef DEBUG
                        ESP_LOGI("Button", "MINUTY SRODKOWY ODKLIKNIETY");
#endif
#ifdef KILL_INSTEAD_OF_HALT
                        xTaskCreatePinnedToCore(dummy, "Clock", CLOCK_STACK, a, CLOCK_PRIORITY, t, CORE);
#else
                        vTaskResume(*(t));
#endif  
                        editMode = EDIT_MODE_NONE; 
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(BUTTON_CLICKED(BR)) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(BUTTON_NOTCLICKED(BR)) state = 9;
                break;  

                case(9):
                    if(BUTTON_NOTCLICKED(BR)) 
                    {
#ifdef DEBUG
                        ESP_LOGI("Button", "MINUTY PRAWY ODKLIKNIETY");
#endif
                        clock_add_minute(a);
                        max7219_displayTime( (uint8_t *)a->timeChars);
                        state = 0;
                    }
                    else state = 8;
                break;

                default:
                    state = 0;
                break;
            }
        }
        else
        {
            BLOCK_UNTIL(editMode = EDIT_MODE_NONE,a->ready);
        }
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(DEBOUNCE_TIME_MS));
    }
}

esp_err_t buttons_init()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BL) | (1ULL << BR) | (1ULL << BC),
        .mode = GPIO_MODE_INPUT,                 // jako wejscie
        .pull_up_en = GPIO_PULLUP_ENABLE,        // z pull-upem
        .pull_down_en = GPIO_PULLDOWN_DISABLE,   // bez pull-downa
        .intr_type = GPIO_INTR_DISABLE,          // Brak przerwań
    };
    return gpio_config(&io_conf);
}

