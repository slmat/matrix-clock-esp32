
#define MINI // poza zamiana MINI na wroom trzeba takze w espidf postawiac kilka rzeczy zeby dzialalo
//#define WROOM
#define DEBUG
//#define KILL_INSTEAD_OF_HALT

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
#endif

#ifndef MAX_COUNT
    #define MAX_COUNT 4
    #define MAX_DATA_SIZE_BYTES MAX_COUNT*16
#endif
#ifndef ROWS
    #define ROWS 8
#endif

#define DISPLAY_STACK 4096