#include "buttons.h"


void buttons_updateLoop(Clock *a, TaskHandle_t *t) 
{
    TickType_t last_wake_time = xTaskGetTickCount();
    enum State editMode = EDIT_MODE_NONE;
    uint8_t state = 0;
    
    while(1)
    {
        if(!a->ready){
            a->ready = 1;
        if(editMode == EDIT_MODE_NONE)
        {
            switch(state)
            {
                case(0):
                if(gpio_get_level(BL) == 0) state = 1;
                else if(gpio_get_level(BC) == 0) state = 4;
                else if(gpio_get_level(BR) == 0) state = 7; 
                break;

                case(1):
                    if(gpio_get_level(BL) == 0) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(gpio_get_level(BL) == 1) state = 3;
                break;

                case(3):
                    if(gpio_get_level(BL) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "JASNOSC LEWY ODKLIKNIETY");
                        #endif
                        if(a->intensity > 0x00)
                        a->intensity -= 1;
                        max7219_changeIntensity(a->intensity);
                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(gpio_get_level(BC) == 0) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(gpio_get_level(BC) == 1) state = 6;
                break;

                case(6):
                    if(gpio_get_level(BC) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "JASNOSC SRODKOWY ODKLIKNIETY"); 
                            vTaskDelete(*(t)); 
                        #else
                            #ifdef KILL_INSTEAD_OF_HALT
                                vTaskDelete(*(t));
                            #else
                                vTaskSuspend(*(t));
                            #endif
                        #endif
                        subM(a);
                        max7219_displayTime((uint8_t *)a->timeChars);
                        max7219_underline(1);
                        editMode = EDIT_MODE_HOURS;
                        a->ready = 1;
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(gpio_get_level(BR) == 0) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(gpio_get_level(BR) == 1) state = 9;
                break;  

                case(9):
                    if(gpio_get_level(BR) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "JASNOSC PRAWY ODKLIKNIETY");
                        #endif
                        if(a->intensity < 0x0f)
                            a->intensity += 1;
                        max7219_changeIntensity( a->intensity); 
                        state = 0;
                    }
                    else state = 8;
                break;

                default:
                    state = 0;
                break;
            }
        }
        else if(editMode == EDIT_MODE_HOURS)
        {
            switch(state)
            {
                case(0):
                if(gpio_get_level(BL) == 0) state = 1;
                else if(gpio_get_level(BC) == 0) state = 4;
                else if(gpio_get_level(BR) == 0) state = 7; 
                break;

                case(1):
                    if(gpio_get_level(BL) == 0) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(gpio_get_level(BL) == 1) state = 3;
                break;

                case(3):
                    if(gpio_get_level(BL) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "GODZINY LEWY ODKLIKNIETY");
                        #endif
                        subH(a);
                        max7219_displayTime( (uint8_t *)a->timeChars); 
                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(gpio_get_level(BC) == 0) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(gpio_get_level(BC) == 1) state = 6;
                break;

                case(6):
                    if(gpio_get_level(BC) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "GODZINY SRODKOWY ODKLIKNIETY"); 
                        #endif
                        max7219_underline( 2);
                        editMode = EDIT_MODE_MINUTES; 
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(gpio_get_level(BR) == 0) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(gpio_get_level(BR) == 1) state = 9;
                break;  

                case(9):
                    if(gpio_get_level(BR) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "GODZINY PRAWY ODKLIKNIETY");
                        #endif
                        addH(a);
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
        else if(editMode == EDIT_MODE_MINUTES)
        {
            switch(state)
            {
                case(0):
                if(gpio_get_level(BL) == 0) state = 1;
                else if(gpio_get_level(BC) == 0) state = 4;
                else if(gpio_get_level(BR) == 0) state = 7; 
                break;

                case(1):
                    if(gpio_get_level(BL) == 0) state = 2;
                    else state = 0;
                break;

                case(2):
                    if(gpio_get_level(BL) == 1) state = 3;
                break;

                case(3):
                    if(gpio_get_level(BL) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "MINUTY LEWY ODKLIKNIETY");
                        #endif
                        subM(a); 
                        max7219_displayTime( (uint8_t *)a->timeChars);
                        state = 0;
                    }
                    else state = 2;
                break;

                case(4):
                    if(gpio_get_level(BC) == 0) state = 5;
                    else state = 0;
                break;

                case(5):
                    if(gpio_get_level(BC) == 1) state = 6;
                break;

                case(6):
                    if(gpio_get_level(BC) == 1) 
                    {
                        a->ready = 1;
                        max7219_underline(0); 
                        a->ready = 0;
                        #ifdef DEBUG
                            ESP_LOGI("Button", "MINUTY SRODKOWY ODKLIKNIETY");
                            xTaskCreatePinnedToCore(dummy, "dummy", 2048, &*(a), 1, t, 0);
                        #else
                            #ifdef KILL_INSTEAD_OF_HALT
                                xTaskCreatePinnedToCore(dummy, "dummy", DISPLAY_STACK, a, 1, t, 0);
                            #else
                                vTaskResume(*(t));
                            #endif  
                        #endif
                        //while(a->ready) {}

                        editMode = EDIT_MODE_NONE; 
                        state = 0;
                    }
                    else state = 5;
                break;

                case(7):
                    if(gpio_get_level(BR) == 0) state = 8;
                    else state = 0;
                break;

                case(8):
                    if(gpio_get_level(BR) == 1) state = 9;
                break;  

                case(9):
                    if(gpio_get_level(BR) == 1) 
                    {
                        #ifdef DEBUG
                            ESP_LOGI("Button", "MINUTY PRAWY ODKLIKNIETY");
                        #endif
                        addM(a);
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
            editMode = EDIT_MODE_NONE;
            a->ready = 0;
        }
        #ifdef DEBUG
            /*ESP_LOGI("READY", "%d", a->ready);*/
        #endif
        a->ready = 0;
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(20));
        }
        else
        {vTaskDelay(pdMS_TO_TICKS(2));};
    }
}

void buttons_init()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BL) | (1ULL << BR) | (1ULL << BC),
        .mode = GPIO_MODE_INPUT,                 // jako wejscie
        .pull_up_en = GPIO_PULLUP_ENABLE,        // z pull-upem
        .pull_down_en = GPIO_PULLDOWN_DISABLE,   // bez pull-downa
        .intr_type = GPIO_INTR_DISABLE,          // Brak przerwań
    };
    gpio_config(&io_conf);
}

