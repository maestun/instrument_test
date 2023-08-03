#include <string.h>
#include <stdio.h>

#include "instrument.h"
#include "mock.h"

#ifdef ENABLE_INSTRUMENTATION

// add your intrument names here...
const char* kInstrumentNames[MAX_INSTRUMENTS] = {
    "sys_tick",
    "analog_sample",
    "",
    "",
    "",
    "",
    "",
    ""
};

static const uint32_t   kMaxFrameDurationMicros = (uint32_t)(DT * 1000000);
static uint32_t         sLogEveryMicros         = (uint32_t)(LOG_INSTRUMENTS_EVERY_MILLIS * 1000);
log_fptr                sLogger                 = NULL;


typedef struct {
    struct {
        uint32_t current;
        uint32_t min;
        uint32_t max;
        uint32_t sum;
    } frameskip;
    struct {
        uint32_t current;
        uint32_t min;
        uint32_t max;
        uint32_t sum;
    } micros;
    uint32_t calls;
    uint32_t last_log_us;
} sInstrumentResult;

static sInstrumentResult sResults[MAX_INSTRUMENTS] = {0};

void instrument_setup_logger(log_fptr f, int log_every_ms) {
    sLogger = f;
    sLogEveryMicros = log_every_ms * 1000;
}

void instrument_tick(eInstrument id) {
    // reset instrument if id is valid
    if(id < MAX_INSTRUMENTS) {
        sResults[id].micros.current = get_timestamp_us();
    }
}

void instrument_tock(eInstrument id) {
    if(id < MAX_INSTRUMENTS) {
        sInstrumentResult *ptr = &sResults[id];
        ptr->calls++;
        uint32_t fs_current = 0;

        // handle the possibility that the timer2 overflows, since its 32 bit only
        uint32_t frame_us = get_delta_us(ptr->micros.current);

        // count number of frames skipped
#ifdef USE_DIVISION
        if(frame_us > kMaxFrameDurationMicros) {
            fs_current = frame_us / kMaxFrameDurationMicros;
            ptr->frameskip.current = fs_current;
            ptr->frameskip.sum += fs_current;
            ptr->frameskip.max = MAX(ptr->frameskip.max, fs_current);
            ptr->frameskip.min = MIN(ptr->frameskip.min, fs_current);
        }
#else
        // TODO: multiplication/loop method might be faster than the division method ?
        while((++fs_current * kMaxFrameDurationMicros) < frame_us)
            ;
        if(--fs_current > 0) {
            ptr->frameskip.current = fs_current;
            ptr->frameskip.sum += fs_current;
            ptr->frameskip.max = MAX(ptr->frameskip.max, fs_current);
            ptr->frameskip.min = MIN(ptr->frameskip.min, fs_current);
        }
#endif
        // update micros
        ptr->micros.current = frame_us;
        ptr->micros.sum += frame_us;
        ptr->micros.max = MAX(ptr->micros.max, frame_us);
        ptr->micros.min = MIN(ptr->micros.min, frame_us);

        // do we need to log ?
        if(get_delta_us(ptr->last_log_us) > sLogEveryMicros) {
            ptr->last_log_us = get_timestamp_us();

            // compute averages
            uint32_t fs_avg = ptr->frameskip.sum / ptr->calls;
            uint32_t us_avg = ptr->micros.sum / ptr->calls;
            ptr->frameskip.sum = 0;
            ptr->micros.sum = 0;
            ptr->calls = 0;

            // log
            sLogger("-- Instrument %d (%s) --\n", id, kInstrumentNames[id]);
            sLogger("frameskip min: %u\n", ptr->frameskip.min);
            sLogger("frameskip max: %u\n", ptr->frameskip.max);
            sLogger("frameskip avg: %u\n", fs_avg);
            sLogger("duration min: %u us\n", ptr->micros.min);
            sLogger("duration max: %u us\n", ptr->micros.max);
            sLogger("duration avg: %u us\n", us_avg);
            fflush(stdout);
        }
    }
}

void instrument_reset(eInstrument id) {
    if(id < MAX_INSTRUMENTS) {
        memset(&sResults[id], 0, sizeof(sInstrumentResult));
    }
}

#endif