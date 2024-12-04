/**
 * @file main.c
 * @brief Główny plik programu dla aplikacji zarządzającej zegarem i przyciskami z użyciem ESP-IDF.
 * @author Sławomir Matonóg
 * @author Maciej Szymonek
 * @date 2024-12-04
 */

#include <stdio.h>
#include "esp_log.h"
#include "esp_heap_caps.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_attr.h>

#include "include/timeControl.h"
#include "include/max7219.h"
#include "include/buttons.h"

/**
 * @brief Struktura reprezentująca zegar.
 */
Clock t;

/**
 * @brief Handler zadania zegara.
 */
TaskHandle_t clockHandle;

/**
 * @brief Główna funkcja aplikacji.
 * 
 * Funkcja inicjalizuje moduły zegara oraz przycisków, a następnie tworzy zadanie
 * zegara na określonym rdzeniu. Na końcu wywoływana jest funkcja obsługująca
 * aktualizację przycisków.
 */
void app_main(void)
{
    /**
     * @brief Inicjalizacja zegara.
     * 
     * Funkcja `clock_init` inicjalizuje strukturę zegara.
     * @param t Wskaźnik do struktury zegara.
     */
    ESP_ERROR_CHECK(clock_init(&t));

    /**
     * @brief Inicjalizacja przycisków.
     * 
     * Funkcja `buttons_init` przygotowuje moduł obsługujący przyciski.
     */
    ESP_ERROR_CHECK(buttons_init());

    /**
     * @brief Tworzenie zadania zegara.
     * 
     * Zadanie zegara jest przypisane do określonego rdzenia.
     * @param dummy Funkcja reprezentująca zadanie zegara.
     * @param "Clock" Nazwa zadania.
     * @param DISPLAY_STACK Rozmiar stosu zadania.
     * @param &t Wskaźnik do struktury zegara przekazywany do zadania.
     * @param CLOCK_PRIORITY Priorytet zadania.
     * @param &clockHandle Handler do utworzonego zadania.
     * @param CORE Rdzeń, na którym zadanie ma działać.
     */
    xTaskCreatePinnedToCore(dummy, "Clock", DISPLAY_STACK, &t, CLOCK_PRIORITY, &clockHandle, CORE);

    /**
     * @brief Aktualizacja przycisków.
     * 
     * Funkcja `buttons_update` obsługuje aktualizację stanu przycisków.
     * @param t Wskaźnik do struktury zegara.
     * @param clockHandle Handler do zadania zegara.
     */
    buttons_update(&t, &clockHandle);
}
