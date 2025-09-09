#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <stdbool.h>

/* Forward declaration for host builds */
typedef struct RTC_TypeDef RTC_TypeDef;

#define RTC_BASE 0x40002800u
#define RTC ((RTC_TypeDef *)RTC_BASE)

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_datetime_t;

/** Initialize RTC peripheral. */
bool rtc_init(RTC_TypeDef *rtc);
/** Set current date and time. */
bool rtc_set_datetime(RTC_TypeDef *rtc, const rtc_datetime_t *dt);
/** Retrieve current date and time. */
bool rtc_get_datetime(RTC_TypeDef *rtc, rtc_datetime_t *dt);
/** Configure wakeup timer in seconds. */
bool rtc_set_wakeup(RTC_TypeDef *rtc, uint16_t seconds);
/** Disable wakeup timer. */
void rtc_disable_wakeup(RTC_TypeDef *rtc);

/** Example: basic RTC initialization and set time. */
void rtc_example_basic(void);
/** Example: configure periodic wakeup. */
void rtc_example_wakeup(void);

/* Example usage:
 *   rtc_datetime_t dt = { .year = 2024, .month = 1, .day = 1, };
 *   rtc_init(RTC);
 *   rtc_set_datetime(RTC, &dt);
 *   rtc_set_wakeup(RTC, 60u);
 */

#endif /* RTC_H */
