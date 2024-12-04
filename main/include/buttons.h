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

/** 
 * @file buttons.h
 * 
 * @brief Biblioteka opisująca funkcje obsługi przycisków
 * 
 * 
 */

/*! \brief enumerator przechowyjący stan w którym się znajdujemy
 *
 *  Przechowywane są wartości odpowiadające odpowiednim trybom edycji
 * poprawne wartości: 
 * EDIT_MODE_NONE, 
 * EDIT_MODE_HOURS, 
 * EDIT_MODE_MINUTES
 * 
 * \remark chyba lepiej to było zrobić zwykłym define ale działa
 */
static enum State {EDIT_MODE_NONE, EDIT_MODE_HOURS, EDIT_MODE_MINUTES};

/*! \brief Funkcja zajmująca się ciągłym sprawdzaniem stanu przycisków
 *
 * W środku znajduje się maszyna stanów sprawdzająca który z przycisków został kliknięty i 
 * w jakim kontekscie. tzn czy w trybie działania czy edycji
 * 
 * funkcja wykorzystuje możliwość blokowania struktury Clock (bool ready) 
 * aby zminimalizować możliwość niepoprawnego wysyłania danych do wyświetlacza
 * ale odbywa się to kosztem szybkości działania
 * 
 * \param Clock* wskaźnik do struktury zegara
 * \param TaskHandle* wskaźnik na uchwyt do zadania odpowiedzialnego za aktualizacje wyświetlacza co minutę 
 * 
 *  \remark 270 linijek sprawdzania stanu 3 przycisków, można było to zrobić 
 *  na przerwaniach od gpio, ale wyszłoby na to samo tylko z inna nazwą funkcji
 */
void buttons_update(Clock *a, TaskHandle_t *t);

/*! \brief Funkcja inicjalizująca przyciski
 *
 * Zajmuje się zadeklarowaniem przycisków jako wejść z pullupami na odpowiednich bitach,
 * Nie ma osobnej funkcji dla obsługi każdego przycisku w innym kontekscie
 * 
 * Każdy przycisk jest sprawdzany za pomocą funkcji gpio_get_level(przycisk) która została dołączona z pliku "driver/gpio.h"
 *   
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t buttons_init();

#endif