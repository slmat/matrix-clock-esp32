# matrix-clock-esp32
zegar na matrycy 32x8 na układzie max7219 oraz mikrokontrolerze esp32c3 mini / wroom32 
program został napisany w espidf przy użyciu freertosa do aktualizacji czasu wyświetlanego na ekranie

# jak skompilować
aby program działał, najprościej jest wygenerować własny projekt z konretną plytka w espidf i przerzucić pliki z folderu main, należy jednak pamiętać o zmianie pinów w pliku defines.h oraz dostosowanie f_cpu i rozmiaru flash w sdk configuration editorze

