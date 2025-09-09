#include <assert.h>
#include "rtc.h"

int main(void) {
    rtc_datetime_t dt = {
        .year = 2023,
        .month = 12,
        .day = 31,
        .hours = 23,
        .minutes = 59,
        .seconds = 50,
    };
    rtc_datetime_t out;
    assert(rtc_init((RTC_TypeDef *)0x40002800u));
    assert(rtc_set_datetime((RTC_TypeDef *)0x40002800u, &dt));
    assert(rtc_get_datetime((RTC_TypeDef *)0x40002800u, &out));
    assert(out.year == dt.year);
    assert(rtc_set_wakeup((RTC_TypeDef *)0x40002800u, 10u));
    rtc_disable_wakeup((RTC_TypeDef *)0x40002800u);
    return 0;
}
