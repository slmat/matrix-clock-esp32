
#define MINI // poza zamiana MINI na wroom trzeba takze w espidf postawiac kilka rzeczy zeby dzialalo
//#define WROOM
//#define DEBUG
#define KILL_INSTEAD_OF_HALT
#define SEPARATOR
#define DEBOUNCE_TIME_MS 10
#define CLOCK_PRIORITY 2 // main task podobno ma 1

#ifdef WR0OM
    #ifndef MOSI
        #define MOSI 23
    #endif
    #ifndef CS
        #define CS 5
    #endif
    #ifndef CLK
        #define CLK 18
    #endif
    #define BL 99
    #define BC 99
    #define BR 99
    #define CORE 1
#endif

#ifdef MINI
    #ifndef MOSI
        #define MOSI 6
    #endif
    #ifndef CS
        #define CS 7
    #endif
    #ifndef CLK
        #define CLK 4
    #endif
    #define BL 0
    #define BC 10
    #define BR 2
    #define CORE 0
#endif

#ifndef MAX_COUNT
    #define MAX_COUNT 4
    #define MAX_DATA_SIZE_BYTES MAX_COUNT*16
#endif
#ifndef ROWS
    #define ROWS 8
#endif

#define DISPLAY_STACK 4096

#define CONDITION_CHECK_INTERVAL_MS 2
#define BLOCK_UNTIL(action,bool_condition)  while(!(bool_condition)){vTaskDelay(pdMS_TO_TICKS(CONDITION_CHECK_INTERVAL_MS));};    \
                                            bool_condition = !bool_condition;   \
                                            action;                             \
                                            bool_condition = !bool_condition  
  