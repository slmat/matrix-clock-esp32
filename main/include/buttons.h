#ifndef BUTTONS_H
#define BUTTONS_H

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "defines.h"
#include "timeControl.h"

#define BUTTON_CLICKED(x) gpio_get_level(x) == 0
#define BUTTON_NOTCLICKED(x) gpio_get_level(x) == 1

/*! \brief enumerator przechowyjący stan w którym się znajdujemy
 *
 *  Przechowywane są wartości odpowiadające odpowiednim trybom edycji
 * poprawne wartości: 
 * EDIT_MODE_NONE, 
 * EDIT_MODE_HOURS, 
 * EDIT_MODE_MINUTES
 */
static enum State {EDIT_MODE_NONE, EDIT_MODE_HOURS, EDIT_MODE_MINUTES};

/*! \brief Funkcja zajmująca się ciągłym sprawdzaniem stanu przycisków
 *
 * W środku znajduje się maszyna stanów sprawdzająca który z przycisków został kliknięty i 
 * w jakim kontekscie. tzn czy w trybie działania czy edycji
 * 
 * \param Clock* wskaźnik do struktury zegara
 * \param TaskHandle* wskaźnik na uchwyt do zadania odpowiedzialnego za aktualizacje wyświetlacza co minutę 
 * 
 *  \note funkcja wykorzystuje możliwość blokowania struktury Clock (bool ready) 
 * aby zminimalizować możliwość niepoprawnego wysyłania danych do wyświetlacza
 * ale odbywa się to kosztem szybkości działania
 */
void buttons_update(Clock *a, TaskHandle_t *t);

/*! \brief Funkcja inicjalizująca przyciski
 *
 * Zajmuje się zadeklarowaniem przycisków jako pullupów na odpowiednich bitach
 * zadeklarowanych w pliku 
 * defines.h
 *  
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t buttons_init();

#endif