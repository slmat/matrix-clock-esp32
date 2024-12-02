#ifndef MAX7219H
#define MAX7219H
#include "esp_log.h"
#include <driver/spi_master.h>
#include <string.h>
#include "defines.h"

extern spi_device_handle_t SPi;

/*! \brief Funkcja zajmująca się inicjalizacją urządzenia na magistrali SPI
 *
 *  Wykorzystuje ona zmienne zadeklarowane w pliku defines.h
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t SPI_init();

/*! \brief Funkcja zajmująca się wysyłaniem tych samych danych do wielu wyświetlaczy
 *
 *  Detailed description starts here.
 * \param register zmienna bajtowa określająca adres na który mają być wysłane dane
 * \param data zmienna bajtowa w której znajdują się dane
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_sendm(const uint8_t reg, const uint8_t data);

/*! \brief Funkcja zajmująca się wysyłaniem wielu danych
 *
 *  Do funkcji należy podać wskaźnik na pierwszy bajt danych do wysłania
 * \param register zmienna bajtowa określająca adres na który mają być wysłane dane
 * \param data* wskaźnik na pierwszy bajt danych do wysłania
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_sendrow(const uint8_t reg, const uint8_t * data);

/*! \brief Funkcja zajmująca się wyświetlaniem tablicy bajtów
 *
 *  Do funkcji należy podać wskaźnik na pierwszy bajt danych do wysłania
 *  Poprawny rozmiar tablicy danych to [4][8] - 4 wyświetlacze po 8 rzędów
 *  
 * \param data* wskaźnik na pierwszy bajt danych do wyświetlenia
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_displayTime(const uint8_t *time);

/*! \brief Funkcja zajmująca się zmianą jasności wyświetlacza
 *
 *  Do funkcji należy podać wartość jasności jaką chcemy osiągnąć
 *  Przy czym warto pamiętać że wartość ta powinna być z zakresu 0x00 - 0xXF
 *  i oznacza ona jak bardzo wypełniony sygnał PWM będą otrzymywać diody
 *  0xXF oznacza wypełnienie 31/32
 *  0x00 oznacza wypełnienie 1/32
 * 
 * \param b intensywność oświetlenia (0-15)
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_changeIntensity(const uint8_t b);

/*! \brief Funkcja zajmująca się podkreślaniem liczb na wyświetlaczu
 *
 *  Do funkcji należy podać którą część wyświetlacza chcemy podkreślić
 *  0 - chcemy wyczyścić ostatni wiersz wszystkich wyświetlaczy
 *  1 - oznacza podkreślenie części godzinowej
 *  2 - oznacza podkreślenie części minutowej
 * 
 * \param bits zmienna bajtowa określająca jaką część wyświetlacza chcemy podkreślić
 * 
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_underline(uint8_t bits);

/*! \brief Funkcja zajmująca się inicjalizacją wyświetlacza max7219
 *
 *  Wykonywanie funkcji zaczyna się od inicjalizacji magistrali SPI
 *  Następnie na wyświetlacz wysyłane są dane potrzebne do inicjalizaji
 *  to jest jasnosc, shutdown, decode, scan limit i display test
 *  jasnosc - poczatkowa wartosc z jaka ma swiecic matryca
 *  shutdown - w tym trybie jasnosc wyswietlacza nie ma znaczenia bo wszystkie diody są wyłączone
 *  decode - tryb bcd wysylanych danych
 *  scan limit - ile bitow wiersza chcemy uzywac do wyswietlania
 *  display test - czy chcemy sprawdzic poprawnosc dzialania wyswietlacza
 *  
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_init();

/*! \brief Funkcja zajmująca się wygaszeniem wszystkich diod wyświetlacza max7219
 *
 *  Funkcja wysyła na każdy wiersz każdego wyświetlacza puste dane
 *  
 * \return zwraca kod błędu po wykonaniu
 */
esp_err_t max7219_clear();


#endif