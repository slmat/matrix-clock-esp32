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

/** 
 * @file defines.h
 * 
 * @brief plik opsiujący wszystkie makra wykorzystywane przez wiele plików
 * 
 * 
 */

/// @defgroup Configuration Konfiguracja
/// @{

/// @brief MINI oznacza urządzenie pod które skonfigurowano wyprowadzenia i działanie. Tutaj oznacza esp32c3 super mini zamiast tego mozna uzyc WROOM
#define MINI 

/// @brief WROOM oznacza urządzenie pod pod które skonfigurowano wyprowadzenia i działanie. Tutaj oznacza esp32 wroom zamiast tego mozna uzyc MINI
//#define WROOM

/// @brief Makro odpowiedzialne za tryb debugowania, włącza komentarze w funkcjach 
//#define DEBUG

/// @brief Opcja stworzona do debugowania, pozwala na zatrzymanie taska zegara zamiast jego restartowanie
#define KILL_INSTEAD_OF_HALT

/// @brief użycie tego makra sprawia że na wyświetlaczu godziny są oddzielone od minut znakiem ':' 
#define SEPARATOR

/// @brief Czas który mija pomiędzy sprawdzaniem stanu styków, w celu zapobiegnieciu ich drgań
#define DEBOUNCE_TIME_MS 10

/// @brief Priorytet taska zegara, większy od funkcji main ponieważ chcemy mieć pewność że sprawdznie przycisków nie przeszokdzi w poprawnym odliczaniu czasu
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
    #define BL 99  ///< Makro określające pin przycisku lewego dla WROOM.
    #define BC 99  ///< Makro określające pin przycisku środkowego dla WROOM.
    #define BR 99  ///< Makro określające pin przycisku prawego dla WROOM.
    #define CORE 1 ///< Rdzeń na którym ma być wykonywane zadanie zegara w przypadku użycia urządzenia WROOM
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
    #define BL 0   ///< Makro określające pin przycisku lewego dla MINI.
    #define BC 10  ///< Makro określające pin przycisku środkowego dla MINI.
    #define BR 2   ///< Makro określające pin przycisku prawego dla MINI.
    #define CORE 0 ///< Rdzeń na którym ma być wykonywane zadanie zegara w przypadku użycia urządzenia MINI.
#endif

/// @brief Maksymalna liczba wyświetlaczy podłączonych do jednego interfejsu spi, odpowiada bezpośrednio za rozmiar danych wysyłanych w pojedynczej transakcji
#ifndef MAX_COUNT
    #define MAX_COUNT 4
    /// @brief Maksymalny rozmiar danych w bajtach wysyłanych w jednej transakcji.
    #define MAX_DATA_SIZE_BYTES MAX_COUNT*16
#endif

/// @brief Liczba wierszy w wyświetlaczu.
#ifndef ROWS
    #define ROWS 8
#endif

/// @brief Rozmiar stosu dla zadania zegara w bajtach.
#define CLOCK_STACK 4096

/// @brief makro używane podczas blokowania wątku w celu zapewnienia warunku, odpowiada za czas ponownego sprawdzenia.
#define CONDITION_CHECK_INTERVAL_MS 2

/**
 * @brief Makro blokujące do momentu spełnienia warunku.
 * 
 * pętla while(!warunek){};
 * 
 * @param action Akcja wykonywana po spełnieniu warunku.
 * @param bool_condition Warunek logiczny.
 */
#define BLOCK_UNTIL(action, bool_condition)  while(!(bool_condition)){vTaskDelay(pdMS_TO_TICKS(CONDITION_CHECK_INTERVAL_MS));};    \
                                            bool_condition = !bool_condition;   \
                                            action;                             \
                                            bool_condition = !bool_condition  
/// @}
