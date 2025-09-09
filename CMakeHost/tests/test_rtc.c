#include <assert.h>
#include "rtc.h"
#include "rcc.h"

int main(void) {
    volatile uint32_t rtc_mem[6] = {0};
    RTC_TypeDef *rtc = (RTC_TypeDef *)rtc_mem;
    rtc_datetime_t dt = {
        .year = 2023,
        .month = 12,
        .day = 31,
        .hours = 23,
        .minutes = 59,
        .seconds = 50,
    };
    rtc_datetime_t out;
    assert(rtc_init(rtc));
    assert(rtc_set_datetime(rtc, &dt));
    assert(rtc_get_datetime(rtc, &out));
    assert(out.year == dt.year);
    assert(rtc_set_wakeup(rtc, 10u));
    rtc_disable_wakeup(rtc);
    return 0;
}
