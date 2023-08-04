#include <string.h>

#include "instrument.h"

#ifdef ENABLE_INSTRUMENTATION

static const uint32_t   kMaxFrameDurationMicros = (uint32_t)(DT * 1000000);
log_fptr                sLog                    = NULL;

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

    struct {
        uint32_t count;
        uint32_t interval_us;
        uint32_t last_us;
    } log;
    
    char name[16];
    uint32_t calls;
} sInstrumentResult;

static sInstrumentResult sResults[MAX_INSTRUMENTS] = {0};

void instrument_set_logger(log_fptr f) {
    sLog = f;
}

void instrument_tick(eInstrument id) {
    if(id < MAX_INSTRUMENTS) {
        sResults[id].micros.current = get_timestamp_us();
    }
}

void instrument_tock(eInstrument id) {
    if(id < MAX_INSTRUMENTS) {
        sInstrumentResult *res = &sResults[id];
        res->calls++;
        uint32_t fs_current = 0;

        // handle the possibility that the timer2 overflows, since its 32 bit only
        uint32_t frame_us = get_delta_us(res->micros.current);

        // count number of frames skipped
#ifdef ENABLE_INSTRUMENTATION_OPTIM
        // multiplication/loop method might be faster than the division method ?
        while((++fs_current * kMaxFrameDurationMicros) < frame_us)
            ;
        if(--fs_current > 0) {
            res->frameskip.current = fs_current;
            res->frameskip.sum += fs_current;
            res->frameskip.max = MAX(res->frameskip.max, fs_current);
            res->frameskip.min = MIN(res->frameskip.min, fs_current);
        }
#else
        if(frame_us > kMaxFrameDurationMicros) {
            fs_current = frame_us / kMaxFrameDurationMicros;
            res->frameskip.current = fs_current;
            res->frameskip.sum += fs_current;
            res->frameskip.max = MAX(res->frameskip.max, fs_current);
            res->frameskip.min = MIN(res->frameskip.min, fs_current);
        }
#endif
        // update micros
        res->micros.current = frame_us;
        res->micros.sum += frame_us;
        res->micros.max = MAX(res->micros.max, frame_us);
        res->micros.min = MIN(res->micros.min, frame_us);

        // do we need to log ?
        if(get_delta_us(res->log.last_us) > res->log.interval_us) {
            res->log.last_us = get_timestamp_us();

            // compute averages
            uint32_t fs_avg = res->frameskip.sum / res->calls;
            uint32_t us_avg = res->micros.sum / res->calls;
            res->frameskip.sum = 0;
            res->micros.sum = 0;
            res->calls = 0;

            // log
            sLog("-- Instrument %d (%s) log #%u --\n", id, res->name, res->log.count++);
            sLog("frameskip min: %u\n", res->frameskip.min);
            sLog("frameskip max: %u\n", res->frameskip.max);
            sLog("frameskip avg: %u\n", fs_avg);
            sLog("duration min: %u us\n", res->micros.min);
            sLog("duration max: %u us\n", res->micros.max);
            sLog("duration avg: %u us\n", us_avg);
            sLog("\n");
        }
    }
}

void instrument_init(eInstrument id, char * name, uint32_t log_interval_millis) {
    if(id < MAX_INSTRUMENTS) {
        memset(&sResults[id], 0, sizeof(sInstrumentResult));
        sResults[id].frameskip.min = UINT32_MAX;
        sResults[id].micros.min = UINT32_MAX;
        sResults[id].log.interval_us = log_interval_millis * 1000;
        strcpy(sResults[id].name, name);
    }
}

#endif