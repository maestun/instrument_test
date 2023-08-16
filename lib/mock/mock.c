#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "mock.h"

static timestamp_t ts = {
    .seconds = 0,
    .microseconds = 0
};

void random_delay_us(int max) {
    int delay = rand() % max;
    usleep(max < 0 ? 100 : delay);
}

void logger(char* str, ...) {
    va_list args;
    va_start(args, str);
    vfprintf(stdout, str, args);
    va_end(args);
}

void flusher(void) {
    fflush(stdout);
}

timestamp_t * get_timestamp() {
    // TODO: implementation

    return &ts;
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
