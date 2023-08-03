#include "mock.h"
#include "instrument.h"


int main(void) {
    instrument_setup_logger(logger, 2000);
    while (1) {
        for(eInstrument id = 0; id < eInstrumentLast; id++) {
            instrument_tick(id);
            random_delay_us(100); // 0...99
            instrument_tock(id);
        }
    }
    return 0;
}