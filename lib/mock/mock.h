#pragma once

#include <stdint.h>

typedef struct {
    uint32_t seconds;
    uint32_t microseconds;
} timestamp_t;

timestamp_t * get_timestamp();
uint32_t get_timestamp_us();

void logger(char* str, ...);
void flusher(void);
void random_delay_us(int max);
