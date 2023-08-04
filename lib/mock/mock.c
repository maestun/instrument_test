#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "mock.h"

void random_delay_us(int max) {
    int delay = rand() % max;
    usleep(max < 0 ? 100 : delay);
}

void logger(char* str, ...) {
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    fflush(stdout);
    va_end(args);
}

// get timestamp from program start
uint32_t get_timestamp_us() {
    static unsigned long long startup_us = 0;
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long long timestamp_micros = currentTime.tv_sec * 1000000LL + currentTime.tv_usec;
    if(startup_us == 0) {
        startup_us = currentTime.tv_sec * 1000000LL + currentTime.tv_usec;
    }
    return (uint32_t)((currentTime.tv_sec * 1000000LL) + currentTime.tv_usec - startup_us);
}

uint32_t get_delta_us(uint32_t us) {
    // handle 32bit overflow
    return MAX(get_timestamp_us() - us, 
               UINT32_MAX - us + get_timestamp_us());
}