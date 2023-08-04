#pragma once

#include <stdint.h>

uint32_t get_timestamp_us();

void logger(char* str, ...);

void random_delay_us(int max);
