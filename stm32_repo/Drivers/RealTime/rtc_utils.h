#ifndef RTC_UTILS_H
#define RTC_UTILS_H

#include <stdint.h>

/** Convert binary value to BCD. */
static inline uint8_t rtc_bin2bcd(uint8_t val) {
    return (uint8_t)(((val / 10u) << 4) | (val % 10u));
}

/** Convert BCD value to binary. */
static inline uint8_t rtc_bcd2bin(uint8_t val) {
    return (uint8_t)(((val >> 4) * 10u) + (val & 0x0Fu));
}

#endif /* RTC_UTILS_H */
