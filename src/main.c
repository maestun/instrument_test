#include "mock.h"
#include "instrument.h"

#define _20_KHZ_ (20000)

int main(void) {
    instrument_setup(_20_KHZ_, logger, flusher, get_timestamp_us);

    instrument_init(eInstrumentSysTick, "sys_tick", 2000);
    instrument_init(eInstrumentAnalogSample, "analog_sampler", 1000);
    instrument_init(eInstrumentHelloWorld, "hello_world", 500);
    
    while (1) {
        instrument_tick(eInstrumentSysTick);
        random_delay_us(-1); // will return 100
        
        instrument_tick(eInstrumentAnalogSample);
        random_delay_us(-1);

        instrument_tock(eInstrumentAnalogSample);

        // test protection
        instrument_tick(eInstrumentSysTick);

        // test nesting
        instrument_tick(eInstrumentHelloWorld);
        random_delay_us(-1);

        instrument_tock(eInstrumentSysTick);
        instrument_tock(eInstrumentHelloWorld);
        
        // test protection
        instrument_tock(eInstrumentHelloWorld);
    }
    return 0;
}