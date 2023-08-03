#pragma once

#include <stdint.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

uint32_t get_timestamp_us();
uint32_t get_delta_us(uint32_t us);

void logger(char* str, ...);

void random_delay_us(int max);
