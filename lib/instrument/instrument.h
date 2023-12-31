
#pragma once

#include <stdint.h>

typedef void (*log_fptr_t)(char*, ...);
typedef void (*flush_fptr_t)(void);
typedef uint32_t (*ts_fptr_t)(void);


#ifdef ENABLE_INSTRUMENTATION

// feel free to configure this
#define MAX_INSTRUMENTS     (8)
#define MAX_INSTRUMENT_NAME (16)

typedef enum {
    // add your intruments here...
    eInstrumentSysTick = 0,
    eInstrumentAnalogSample,
    eInstrumentHelloWorld,
    eInstrument4,
    eInstrument5,
    eInstrument6,
    eInstrument7,

    // ...but never after here.
    eInstrumentLast = MAX_INSTRUMENTS
} eInstrument;

/**
 * Set the global function pointers. Call this before everyting else
 * @param expected_frame_freq_hz the expented frame frequency before considering frameskip
 * @param logf the logger funtion pointer (ie. 'printf'...)
 * @param flushf the buffer flushing funtion pointer (ie. 'fflush'...)
 * @param tsf the microseconds timestamp funtion pointer (ie. 'get_ts'...) 
 */
void instrument_setup(uint32_t expected_frame_freq_hz, 
                      log_fptr_t logf, 
                      flush_fptr_t flushf, 
                      ts_fptr_t tsf);


/**
 * Setup some parameters for a given instrument
 * @param id The instrument identifier
 * @param name MAX_INSTRUMENT_NAME characters, name to be displayed by the logger
 * @param log_interval_millis Call logger every xxx milliseconds
 */
void instrument_init(eInstrument id, 
                     char * name, 
                     uint32_t log_interval_millis);


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

#define instrument_setup(expected_frame_freq_hz, logf, flushf, tsf)
#define instrument_tick(id)
#define instrument_tock(id)
#define instrument_init(id, name, millis)

#endif
