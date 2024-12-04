#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "max7219.h"

/** 
 * @file timeControl.h
 * 
 * @brief Biblioteka opisująca funkcje zegara
 * 
 * 
 */
/**
 *  
 * @brief Tablica przechowująca cyfry 0-9 w postaci bitowej.
 * 
 * Zawiera graficzną reprezentację każdej cyfry w rozmiarze 8x8 bitów.
 * 
 * @note Funkcja wykorzystuje możliwość zapisywania w pamięci programu, aby zoptymalizować działanie.
 * @note Static const automatycznie umieszcza dane w pamięci programu w ESP-IDF, więc dodatkowy atrybut nie jest wymagany.
 */
static const uint8_t digits[10][8] __attribute__((section(".rodata"))) = {
    { 0x3C, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x3C, 0x00 }, // 0
    { 0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x3E, 0x00 }, // 1
    { 0x3C, 0x42, 0x02, 0x0C, 0x30, 0x40, 0x7E, 0x00 }, // 2
    { 0x3C, 0x42, 0x02, 0x1C, 0x02, 0x42, 0x3C, 0x00 }, // 3
    { 0x04, 0x0C, 0x14, 0x24, 0x7E, 0x04, 0x04, 0x00 }, // 4
    { 0x7E, 0x40, 0x7C, 0x02, 0x02, 0x42, 0x3C, 0x00 }, // 5
    { 0x3C, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x3C, 0x00 }, // 6
    { 0x7E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x00 }, // 7
    { 0x3C, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x3C, 0x00 }, // 8
    { 0x3C, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x3C, 0x00 }, // 9
};

/**
 * @brief Struktura przechowująca aktualny czas.
 * 
 * Struktura zawiera zmienne potrzebne do poprawnego wyświetlania i aktualizacji czasu
 * 
 * @note Flaga `ready` pozwala minimalizować błędy podczas wysyłania danych do wyświetlacza, 
 * ale nie jest wymagana gdy nie mamy zamiaru bawić się przyciskami a po prostu ustawić czas 
 */
typedef struct {
    uint8_t time[4];        /**< Aktualny czas w formacie liczbowym. */
    uint8_t timeChars[4][8];/**< Graficzna reprezentacja czasu. */
    bool ready;             /**< Flaga gotowości do aktualizacji wyświetlacza. */
    uint8_t intensity;      /**< Jasność wyświetlacza (0-15). */
} Clock;

/**
 * @brief Inicjalizuje wartości początkowe zegara.
 * 
 * Nadpisuje tablicę znaków w strukturze Clock, aby od razu po włączeniu wyświetlać czas.
 * 
 * @param a Wskaźnik na strukturę zegara.
 * @return Kod błędu w przypadku problemów.
 */
esp_err_t clock_init(Clock *a);

/**
 * @brief Zwiększa jasność wyświetlacza o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 * @return True, jeśli operacja się powiodła, false w przeciwnym razie.
 */
inline bool clock_inc_intensity(Clock *a);

/**
 * @brief Zmniejsza jasność wyświetlacza o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 * @return True, jeśli operacja się powiodła, false w przeciwnym razie.
 */
inline bool clock_dec_intensity(Clock *a);

/**
 * @brief Ustawia jasność wyświetlacza na podaną wartość.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 * @param i Wartość intensywności (0-15).
 */
inline void clock_set_intensity(Clock *a, const uint8_t i) __attribute__((weak));

/**
 * @brief Aktualizuje graficzną reprezentację cyfry w zegarze.
 * 
 * @param place Miejsce cyfry do odświeżenia (0-3).
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 */
void clock_update_timeChar(const uint8_t place, Clock *a);

/**
 * @brief Zwiększa liczbę minut o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 * @return True, jeśli operacja się powiodła, false w przeciwnym razie.
 */
bool clock_add_minute(Clock *a);

/**
 * @brief Zwiększa liczbę godzin o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 * @return True, jeśli operacja się powiodła, false w przeciwnym razie.
 */
bool clock_add_hour(Clock *a);

/**
 * @brief Zmniejsza liczbę minut o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 */
void clock_sub_minute(Clock *a);

/**
 * @brief Zmniejsza liczbę godzin o 1.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 */
void clock_sub_hour(Clock *a);

/**
 * @brief Aktualizuje czas w zegarze.
 * 
 * Wywołuje funkcje odpowiedzialne za dodanie minuty i ewentualnie godziny.
 * 
 * @param a Wskaźnik na strukturę zegara do aktualizacji.
 */
void clock_update(Clock *a);

/*! \brief Funkcja, która jest nieużywana w aktualnym kodzie
 * 
 * Ta funkcja została zachowana w kodzie ze względu na problem z kompilacją
 * programu, który wystąpił podczas próby użycia jej jako argumentu w funkcji
 * `xTaskCreate`. Po zduplikowaniu i zmianie nazwy na `dummy`, kompilacja
 * przebiegła pomyślnie.
 * 
 * \note Zobacz funkcję `dummy()`.
 */
void Clock_Loop(void *); // deprecated

/*! \brief Funkcja wykonująca zadanie odświeżania wyświetlacza
 * 
 * Co minutę aktualizuje czas w strukturze `Clock` i wyświetlacz
 * 
 * Znajduje się w osobnym tasku tak aby można nią było sterować w przypadku chęci ustawienia czasu
 * 
 * \param a Wskaźnik do struktury `Clock`, na której funkcja ma operować.
 */
void dummy(Clock *); 


#endif