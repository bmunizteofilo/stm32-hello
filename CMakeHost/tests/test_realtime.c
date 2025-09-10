#include <assert.h>
#include "RealTime/rtc_utils.h"
#include "RealTime/ds1307.h"

int main(void) {
    assert(rtc_bin2bcd(45u) == 0x45u);
    assert(rtc_bcd2bin(0x59u) == 59u);
    volatile uint32_t i2c_mem[11] = {0};
    i2c_mem[6] = 0xFFFFFFFFu; /* ISR flags */
    I2C_TypeDef *i2c = (I2C_TypeDef *)i2c_mem;
    rtc_datetime_t dt = { .year = 2024, .month = 1, .day = 2, .hours = 3, .minutes = 4, .seconds = 5 };
    assert(ds1307_set_datetime(i2c, DS1307_I2C_ADDR, &dt));
    assert(ds1307_init(i2c, DS1307_I2C_ADDR));
    return 0;
}
