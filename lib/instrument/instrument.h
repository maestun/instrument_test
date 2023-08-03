
#pragma once

#ifdef __STM32__
#include "tim.h"
#include "hw_config.h"
#else
#include <stdint.h>
#define LOOP_FREQUENCY      20000           // 20 kHz loop freq
#define DT					(1.0 / LOOP_FREQUENCY) // loop period
#endif

#ifdef ENABLE_INSTRUMENTATION

// feel free to configure this
#define MAX_INSTRUMENTS                 (8)
#define LOG_INSTRUMENTS_EVERY_MILLIS    (1000)

typedef enum {
    // add your intruments here...
    eInstrumentSysTick = 0,
    eInstrumentAnalogSample,
    eInstrument3,
    eInstrument4,
    eInstrument5,
    eInstrument6,
    eInstrument7,

    // ...but never after here.
    eInstrumentLast = MAX_INSTRUMENTS
} eInstrument;

typedef void (*log_fptr)(char*, ...);

void instrument_setup_logger(log_fptr f, int log_every_ms);
void instrument_tick(eInstrument id);
void instrument_tock(eInstrument id);
void instrument_reset(eInstrument id);

#else

#define instrument_tick(id)
#define instrument_tock(id)
#define instrument_reset(id)

#endif
