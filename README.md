# matrix-clock-esp32
clock displayed on 8x32 matrix using max7219 and esp32c3 super mini
program został napisany w espidf przy użyciu freertosa do aktualizacji czasu wyświetlanego na ekranie

# jak skompilować
aby program działał, najprościej jest wygenerować własny projekt z konretna plytka z espidf i przerzucić pliki z folderu main, należy jednak pamiętać o zmianie pinów w pliku defines.h oraz dostosowanie f_cpu i rozmiaru flash w sdk configuration editorze
