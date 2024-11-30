#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "max7219.h"

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

typedef struct{
    uint8_t time[4];
    uint8_t timeChars[4][8];
    uint8_t ready;
    uint8_t intensity;
}Clock;

void initT(Clock *a/*, spi_device_handle_t *spi*/);
void UpdateTimeChar(const uint8_t place, Clock *a);
bool addM(Clock *a);
bool addH(Clock *a);
void subM(Clock *a);
void subH(Clock *a);
void count(Clock *a);
void Clock_Loop(void *);
void dummy(Clock *);

#endif