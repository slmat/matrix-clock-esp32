/**
 * @file config.h
 * @brief Konfiguracja preprocesora dla projektu.
 * 
 * Ten plik zawiera makra i stałe używane do konfiguracji sprzętu oraz ustawień aplikacji.
 * 
 * @note Niektóre ustawienia wymagają dodatkowych zmian w ESP-IDF.
 * @author Jan Kowalski
 * @date 2024-12-04
 */

/// @defgroup Configuration Konfiguracja
/// @{

/// @brief Tryb miniaturowy dla urządzenia.
#define MINI 

/// @brief Tryb WROOM (do skonfigurowania w ESP-IDF).
//#define WROOM

/// @brief Włączenie trybu debugowania.
//#define DEBUG

/// @brief Zatrzymanie zadania zamiast jego zabijania.
#define KILL_INSTEAD_OF_HALT

/// @brief Separator danych.
#define SEPARATOR

/// @brief Czas odbijania w ms.
#define DEBOUNCE_TIME_MS 10

/// @brief Priorytet zegara.
#define CLOCK_PRIORITY 2

#ifdef WR0OM
    /// @brief Pin MOSI dla WROOM.
    #ifndef MOSI
        #define MOSI 23
    #endif
    /// @brief Pin CS dla WROOM.
    #ifndef CS
        #define CS 5
    #endif
    /// @brief Pin CLK dla WROOM.
    #ifndef CLK
        #define CLK 18
    #endif
    #define BL 99 ///< Poziom podświetlenia lewej części dla WROOM.
    #define BC 99 ///< Poziom podświetlenia środkowej części dla WROOM.
    #define BR 99 ///< Poziom podświetlenia prawej części dla WROOM.
    #define CORE 1 ///< Rdzeń CPU dla WROOM.
#endif

#ifdef MINI
    /// @brief Pin MOSI dla MINI.
    #ifndef MOSI
        #define MOSI 6
    #endif
    /// @brief Pin CS dla MINI.
    #ifndef CS
        #define CS 7
    #endif
    /// @brief Pin CLK dla MINI.
    #ifndef CLK
        #define CLK 4
    #endif
    #define BL 0 ///< Poziom podświetlenia lewej części dla MINI.
    #define BC 10 ///< Poziom podświetlenia środkowej części dla MINI.
    #define BR 2 ///< Poziom podświetlenia prawej części dla MINI.
    #define CORE 0 ///< Rdzeń CPU dla MINI.
#endif

/// @brief Maksymalna liczba urządzeń w systemie.
#ifndef MAX_COUNT
    #define MAX_COUNT 4
    /// @brief Maksymalny rozmiar danych w bajtach.
    #define MAX_DATA_SIZE_BYTES MAX_COUNT*16
#endif

/// @brief Liczba wierszy w wyświetlaczu.
#ifndef ROWS
    #define ROWS 8
#endif

/// @brief Rozmiar stosu wyświetlacza w bajtach.
#define DISPLAY_STACK 4096

/// @brief Interwał sprawdzania warunku w ms.
#define CONDITION_CHECK_INTERVAL_MS 2

/**
 * @brief Makro blokujące do momentu spełnienia warunku.
 * 
 * @param action Akcja wykonywana po spełnieniu warunku.
 * @param bool_condition Warunek logiczny.
 */
#define BLOCK_UNTIL(action, bool_condition)  while(!(bool_condition)){vTaskDelay(pdMS_TO_TICKS(CONDITION_CHECK_INTERVAL_MS));};    \
                                            bool_condition = !bool_condition;   \
                                            action;                             \
                                            bool_condition = !bool_condition  
/// @}
