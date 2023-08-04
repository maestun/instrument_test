
#pragma once

#ifdef PIO_NATIVE
#include <stdint.h>
#include "mock.h"
#else
#include "tim.h"
#include "hw_config.h"
#endif

typedef void (*log_fptr)(char*, ...);

#ifdef ENABLE_INSTRUMENTATION

// feel free to configure this
#define MAX_INSTRUMENTS                 (8)

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

/**
 * Set the global logger function pointer. Call this before everyting else
 * @param f A function pointer (ie. 'printf'...) 
 */
void instrument_set_logger(log_fptr f);


/**
 * Setup some parameters for a given instrument
 * @param id The instrument identifier
 * @param name Max 16 characters, name to be displayed by the logger
 * @param log_interval_millis Call logger every xxx milliseconds
 */
void instrument_init(eInstrument id, char * name, uint32_t log_interval_millis);


/**
 * Starts the instrument counter
 * @param id The instrument identifier
 */
void instrument_tick(eInstrument id);


/**
 * Stops the instrument counter, and compute values, will call logger at setup intervals
 * @param id The instrument identifier
 */
void instrument_tock(eInstrument id);

#else

#define instrument_set_logger(f)
#define instrument_tick(id)
#define instrument_tock(id)
#define instrument_init(id, name, millis)

#endif
