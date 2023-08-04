#include "mock.h"
#include "instrument.h"


int main(void) {
    instrument_setup(logger, get_timestamp_us);

    instrument_init(eInstrumentSysTick, "sys_tick", 2000);
    instrument_init(eInstrumentAnalogSample, "analog_sampler", 1000);
    
    while (1) {
        instrument_tick(eInstrumentSysTick);
        random_delay_us(-1); // will return 100
        instrument_tock(eInstrumentSysTick);

        instrument_tick(eInstrumentAnalogSample);
        random_delay_us(30); // 0...30
        instrument_tock(eInstrumentAnalogSample);
    }
    return 0;
}