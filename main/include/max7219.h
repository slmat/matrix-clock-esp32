#ifndef MAX7219H
#define MAX7219H
#include "esp_log.h"
#include <driver/spi_master.h>
#include <string.h>
#include "defines.h"

extern spi_device_handle_t SPi;

/**
 * @brief Funkcja inicjalizująca urządzenie na magistrali SPI.
 * 
 * Wykorzystuje zmienne zadeklarowane w pliku defines.h.
 * 
 * @return Kod błędu po wykonaniu operacji.
 */
esp_err_t SPI_init();

/**
 * @brief Funkcja wysyłająca te same dane do wielu wyświetlaczy.
 * 
 * Wysyła dane do określonego rejestru wszystkich podłączonych urządzeń Max7219.
 * 
 * @param reg Bajt określający adres rejestru, na który mają być wysłane dane.
 * @param data Bajt danych do wysłania do wybranego rejestru.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_sendm(const uint8_t reg, const uint8_t data);

/**
 * @brief Funkcja wysyłająca wiele danych do jednego rejestru.
 * 
 * Do funkcji należy podać wskaźnik na pierwszy bajt danych do wysłania.
 * 
 * @param reg Bajt określający adres rejestru, na który mają być wysłane dane.
 * @param data Wskaźnik na pierwszy bajt danych do wysłania.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_sendrow(const uint8_t reg, const uint8_t *data);

/**
 * @brief Funkcja wyświetlająca tablicę bajtów na matrycach LED.
 * 
 * Do funkcji należy podać wskaźnik na pierwszy bajt danych do wyświetlenia. 
 * Poprawny rozmiar tablicy danych to [4][8] - 4 wyświetlacze po 8 rzędów.
 * 
 * @param time Wskaźnik na pierwszy bajt danych do wyświetlenia.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_displayTime(const uint8_t *time);

/**
 * @brief Funkcja zmieniająca jasność wyświetlaczy.
 * 
 * Do funkcji należy podać wartość jasności, która powinna być z zakresu 0x00 - 0x0F. 
 * Wartość określa wypełnienie sygnału PWM sterującego diodami.
 * 
 * @param b Intensywność oświetlenia (0-15).
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_changeIntensity(const uint8_t b);

/**
 * @brief Funkcja podkreślająca wybrane części wyświetlacza.
 * 
 * Funkcja umożliwia podkreślenie określonych sekcji wyświetlacza.
 * 
 * @param bits Bajt określający, którą część wyświetlacza podkreślić:
 * - 0: wyczyść ostatni wiersz wszystkich wyświetlaczy.
 * - 1: podkreśl część godzinową.
 * - 2: podkreśl część minutową.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_underline(uint8_t bits);

/**
 * @brief Funkcja inicjalizująca wyświetlacz Max7219.
 * 
 * Funkcja inicjalizuje magistralę SPI i wysyła dane konfiguracyjne do wyświetlacza, 
 * takie jak jasność, tryb shutdown, dekodowanie, limit skanowania i test wyświetlacza.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_init();

/**
 * @brief Funkcja wygaszająca wszystkie diody wyświetlacza Max7219.
 * 
 * Funkcja wysyła na każdy wiersz wszystkich wyświetlaczy puste dane, co powoduje wygaszenie.
 * 
 * @return 
 * - `ESP_OK` jeśli operacja się powiedzie.
 * - Kod błędu w przypadku problemów.
 */
esp_err_t max7219_clear();

#endif
