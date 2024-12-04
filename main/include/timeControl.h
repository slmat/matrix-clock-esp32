#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "max7219.h"

/** 
 * @brief Tablica przechowująca cyfry 0-9 w postaci bitowej.
 * 
 * Zawiera graficzną reprezentację każdej cyfry w rozmiarze 8x8 bitów.
 * 
 * @note Funkcja wykorzystuje możliwość zapisywania w pamięci programu, aby zoptymalizować działanie.
 * @note Static const automatycznie umieszcza dane w pamięci programu w ESP-IDF, więc dodatkowy atrybut nie jest wymagany.
 */
static const uint8_t digits[10][8] __attribute__((section(".rodata"))) = {
    // Cyfra 0
    {
        0b00111100,
        0b01000010,
        0b01000110,
        0b01001010,
        0b01010010,
        0b01100010,
        0b00111100,
        0x00,
    },
    // Cyfra 1
    {
        0b00001000,
        0b00011000,
        0b00101000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00111110,
        0x00,
    },
    // Cyfra 2
    {
        0b00111100,
        0b01000010,
        0b00000010,
        0b00001100,
        0b00110000,
        0b01000000,
        0b01111110,
        0x00,
    },
    // Cyfra 3
    {
        0b00111100,
        0b01000010,
        0b00000010,
        0b00011100,
        0b00000010,
        0b01000010,
        0b00111100,
        0x00,
    },
    // Cyfra 4
    {
        0b00000100,
        0b00001100,
        0b00010100,
        0b00100100,
        0b01111110,
        0b00000100,
        0b00000100,
        0x00,
    },
    // Cyfra 5
    {
        0b01111110,
        0b01000000,
        0b01111100,
        0b00000010,
        0b00000010,
        0b01000010,
        0b00111100,
        0x00,
    },
    // Cyfra 6
    {
        0b00111100,
        0b01000000,
        0b01111100,
        0b01000010,
        0b01000010,
        0b01000010,
        0b00111100,
        0x00,
    },
    // Cyfra 7
    {
        0b01111110,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
        0b00100000,
        0x00,
    },
    // Cyfra 8
    {
        0b00111100,
        0b01000010,
        0b01000010,
        0b00111100,
        0b01000010,
        0b01000010,
        0b00111100,
        0x00,
    },
    // Cyfra 9
    {
        0b00111100,
        0b01000010,
        0b01000010,
        0b00111110,
        0b00000010,
        0b00000010,
        0b00111100,
        0x00,
    },
};

/**
 * @brief Struktura przechowująca aktualny czas.
 * 
 * Zawiera czas jako liczby i graficzną reprezentację w formie tablicy znaków.
 * Przechowuje również jasność wyświetlacza oraz gotowość do aktualizacji.
 * 
 * @note Flaga `ready` pozwala minimalizować błędy podczas wysyłania danych do wyświetlacza.
 */
typedef struct {
    uint8_t time[4];        /**< Aktualny czas w formacie liczbowym. */
    uint8_t timeChars[4][8];/**< Graficzna reprezentacja czasu. */
    bool ready;             /**< Flaga gotowości do aktualizacji. */
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

/*! \brief Funkcja wykonująca zadanie odświeżania wyświetlacza co minutę
 * 
 * Ta funkcja przyjmuje strukturę `Clock` i wykonuje operacje na jej danych.
 * 
 * \param a Wskaźnik do struktury `Clock`, na której funkcja ma operować.
 */
void dummy(Clock *); 


#endif