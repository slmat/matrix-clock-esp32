#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "max7219.h"

/*! \brief tablica przechowująca cyfry 0 - 9 w postacji bitowej
 *
 * W środku znajduje się graficzna reprezentacja każdej cyfry w rozmiarach 8x8 bitów
 * 
 *  \note funkcja wykorzystuje możliwość zapisywania w pamięci programu aby zoptymalizować działanie
 * \note  i podobno skoro jest static const to espidf automatycznie zapisuje dane w pamięci programu i atrybut nie jest potrzebny
 */
static const uint8_t digits[10][8] __attribute__((section(".rodata"))) =  {
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

/*! \brief Struktura przechowująca aktualny czas
 *
 * W środku znajduje się czas jako liczba oraz jako tablica znaków 
 * struktura przechowuje także jasność wyświetlacza i gotowość do wyświetlania 
 * 
 *  \note funkcja wykorzystuje możliwość blokowania (bool ready) 
 * aby zminimalizować możliwość niepoprawnego wysyłania danych do wyświetlacza
 */
typedef struct{
    uint8_t time[4];
    uint8_t timeChars[4][8];
    bool ready;
    uint8_t intensity;
}Clock;

/*! \brief Funkcja służąca do wstępnej inicjalizacji wartości zegara
 *
 * Zajmuje się nadpisaniem tablicy znaków struktury Clock aby od razu po
 * włączieniu był wyświetlany czas
 * 
 */
esp_err_t clock_init(Clock *a);

/*! \brief Funkcja służąca do zwiększenia intesywności świecenia wyświetlacza o 1
 *
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
inline bool clock_inc_intensity(Clock *a);

/*! \brief Funkcja służąca do zmniejszenia intesywności świecenia wyświetlacza o 1
 *
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
inline bool clock_dec_intensity(Clock *a);

/*! \brief Funkcja służąca do ustawienia intesywności świecenia wyświetlacza o 1
 *
 *  \note Funkja ta posiada atrybut weak tak aby można było ją nadpisać
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować
 *  \param i wartość intensywności świecenia którą chcemy osiągnąć (0-15)  
 */
inline void clock_set_intensity(Clock *a, const uint8_t i) __attribute__((weak));

/*! \brief Funkcja służąca do zaktualizowania tablicy znaków zegara na danej pozycji
 *
 *  \param place (0-3) miejsce cyfry którą ma odświeżyć  
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
void clock_update_timeChar(const uint8_t place, Clock *a);

/*! \brief Funkcja służąca do zwiększenia liczby minut o 1
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
bool clock_add_minute(Clock *a);

/*! \brief Funkcja służąca do zwiększenia liczby godzin o 1
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
bool clock_add_hour(Clock *a);

/*! \brief Funkcja służąca do mniejszenia liczby minut o 1
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
void clock_sub_minute(Clock *a);

/*! \brief Funkcja służąca do zmniejszenia liczby godzin o 1
 * 
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
void clock_sub_hour(Clock *a);

/*! \brief Funkcja służąca do aktualizacji zegara
 * 
 * Wywołuje ona odpowiednio funkcję odpowiedzialne za dodanie minuty
 * i w razie potrzeby za dodanie godziny
 *  \param a przyjmuje strukturę Clock którą ma zaktualizować  
 */
void clock_update(Clock *a);

/*! \brief Funkcja która jest bezużyteczna w obecnym kodzie
 * 
 * ale została dla upamiętnienia faktu iż nie program nie chciał się
 * kompilować gdy użyłem jej jako argumentu w xtaskcreate ale gdy ją zduplikowałem
 * i nazwałem dummy, kod nie miał problemu się skompilować 
 * 
 * \note patrz funkjcę dummy()
 */
void Clock_Loop(void *); // deprecated

/*! \brief Funkcja służąca jako task do odświeżania wyświetlacza co minutę
 * 
 *  \param a przyjmuje strukturę Clock na któej ma operować  
 */
void dummy(Clock *);

#endif