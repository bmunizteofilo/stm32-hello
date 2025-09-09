#include "rtc.h"

#if defined(STM32F0_FIRMWARE)
struct RTC_TypeDef {
    volatile uint32_t TR;
    volatile uint32_t DR;
    volatile uint32_t CR;
    volatile uint32_t ISR;
    volatile uint32_t PRER;
    volatile uint32_t WUTR;
};

bool rtc_init(RTC_TypeDef *rtc) {
    if (!rtc) {
        return false;
    }
    /* In real firmware, enable clock and configure prescalers. */
    rtc->ISR |= 1u; /* just mark initialized */
    return true;
}

bool rtc_set_datetime(RTC_TypeDef *rtc, const rtc_datetime_t *dt) {
    if (!rtc || !dt) {
        return false;
    }
    /* Simplified write of calendar registers without BCD conversion. */
    rtc->TR = ((uint32_t)dt->hours << 16) |
              ((uint32_t)dt->minutes << 8) |
              dt->seconds;
    rtc->DR = ((uint32_t)dt->year << 16) |
              ((uint32_t)dt->month << 8) |
              dt->day;
    return true;
}

bool rtc_get_datetime(RTC_TypeDef *rtc, rtc_datetime_t *dt) {
    if (!rtc || !dt) {
        return false;
    }
    uint32_t tr = rtc->TR;
    uint32_t dr = rtc->DR;
    dt->seconds = tr & 0xFFu;
    dt->minutes = (tr >> 8) & 0xFFu;
    dt->hours   = (tr >> 16) & 0xFFu;
    dt->day     = dr & 0xFFu;
    dt->month   = (dr >> 8) & 0xFFu;
    dt->year    = (uint16_t)(dr >> 16);
    return true;
}

bool rtc_set_wakeup(RTC_TypeDef *rtc, uint16_t seconds) {
    if (!rtc || seconds == 0u) {
        return false;
    }
    rtc->WUTR = seconds;
    rtc->CR |= 1u; /* pretend to enable wakeup */
    return true;
}

void rtc_disable_wakeup(RTC_TypeDef *rtc) {
    if (!rtc) {
        return;
    }
    rtc->CR &= ~1u;
}

void rtc_example_basic(void) {
    rtc_datetime_t dt = {
        .year = 2024,
        .month = 1,
        .day = 1,
        .hours = 0,
        .minutes = 0,
        .seconds = 0,
    };
    rtc_init(RTC);
    rtc_set_datetime(RTC, &dt);
}

void rtc_example_wakeup(void) {
    rtc_init(RTC);
    rtc_set_wakeup(RTC, 5u);
}

#else
static rtc_datetime_t current;
static uint16_t wakeup_seconds;

bool rtc_init(RTC_TypeDef *rtc) {
    (void)rtc;
    current = (rtc_datetime_t){0};
    wakeup_seconds = 0u;
    return true;
}

bool rtc_set_datetime(RTC_TypeDef *rtc, const rtc_datetime_t *dt) {
    (void)rtc;
    if (!dt) {
        return false;
    }
    current = *dt;
    return true;
}

bool rtc_get_datetime(RTC_TypeDef *rtc, rtc_datetime_t *dt) {
    (void)rtc;
    if (!dt) {
        return false;
    }
    *dt = current;
    return true;
}

bool rtc_set_wakeup(RTC_TypeDef *rtc, uint16_t seconds) {
    (void)rtc;
    if (seconds == 0u) {
        return false;
    }
    wakeup_seconds = seconds;
    return true;
}

void rtc_disable_wakeup(RTC_TypeDef *rtc) {
    (void)rtc;
    wakeup_seconds = 0u;
}

void rtc_example_basic(void) {
    rtc_datetime_t dt = {
        .year = 2024,
        .month = 1,
        .day = 1,
        .hours = 0,
        .minutes = 0,
        .seconds = 0,
    };
    rtc_init((RTC_TypeDef *)0);
    rtc_set_datetime((RTC_TypeDef *)0, &dt);
}

void rtc_example_wakeup(void) {
    rtc_init((RTC_TypeDef *)0);
    rtc_set_wakeup((RTC_TypeDef *)0, 5u);
}

#endif
