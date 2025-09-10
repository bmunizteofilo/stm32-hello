#include "RealTime/ds3231.h"
#include <string.h>

static bool ds3231_write(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t buf[8];
    if (len + 1u > sizeof(buf)) return false;
    buf[0] = reg;
    for (size_t i = 0u; i < len; ++i) {
        buf[i + 1u] = data[i];
    }
    return i2c_write_poll(i2c, addr, buf, len + 1u);
}

static bool ds3231_read(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data, size_t len) {
    if (!i2c_write_poll(i2c, addr, &reg, 1u)) return false;
    return i2c_read_poll(i2c, addr, data, len);
}

bool ds3231_set_datetime(I2C_TypeDef *i2c, uint8_t addr, const rtc_datetime_t *dt) {
    if (!dt) return false;
    uint8_t buf[7];
    buf[0] = rtc_bin2bcd(dt->seconds);
    buf[1] = rtc_bin2bcd(dt->minutes);
    buf[2] = rtc_bin2bcd(dt->hours);
    buf[3] = rtc_bin2bcd(1u); /* day of week */
    buf[4] = rtc_bin2bcd(dt->day);
    buf[5] = rtc_bin2bcd(dt->month);
    buf[6] = rtc_bin2bcd((uint8_t)(dt->year % 100u));
    return ds3231_write(i2c, addr, 0x00u, buf, 7u);
}

bool ds3231_get_datetime(I2C_TypeDef *i2c, uint8_t addr, rtc_datetime_t *dt) {
    if (!dt) return false;
    uint8_t buf[7];
    if (!ds3231_read(i2c, addr, 0x00u, buf, 7u)) return false;
    dt->seconds = rtc_bcd2bin(buf[0]);
    dt->minutes = rtc_bcd2bin(buf[1]);
    dt->hours   = rtc_bcd2bin(buf[2] & 0x3Fu);
    dt->day     = rtc_bcd2bin(buf[4]);
    dt->month   = rtc_bcd2bin(buf[5] & 0x1Fu);
    dt->year    = (uint16_t)(2000u + rtc_bcd2bin(buf[6]));
    return true;
}

bool ds3231_read_temperature(I2C_TypeDef *i2c, uint8_t addr, int16_t *temp_c_256) {
    if (!temp_c_256) return false;
    uint8_t buf[2];
    if (!ds3231_read(i2c, addr, 0x11u, buf, 2u)) return false;
    int16_t raw = (int16_t)((buf[0] << 8) | buf[1]);
    *temp_c_256 = raw >> 6; /* 1/4 C -> 1/256 C */
    return true;
}

bool ds3231_set_alarm1_daily(I2C_TypeDef *i2c, uint8_t addr, uint8_t hour, uint8_t minute, uint8_t second) {
    uint8_t buf[4];
    buf[0] = rtc_bin2bcd(second);
    buf[1] = rtc_bin2bcd(minute);
    buf[2] = rtc_bin2bcd(hour);
    buf[3] = 0x80u; /* match every day */
    if (!ds3231_write(i2c, addr, 0x07u, buf, 4u)) return false;
    uint8_t ctrl;
    if (!ds3231_read(i2c, addr, 0x0Eu, &ctrl, 1u)) return false;
    ctrl |= 0x05u; /* enable alarm1 + INTCN */
    return ds3231_write(i2c, addr, 0x0Eu, &ctrl, 1u);
}

void ds3231_example(void) {
    rtc_datetime_t dt = { .year = 2024, .month = 1, .day = 1, .hours = 0, .minutes = 0, .seconds = 0 };
    ds3231_set_datetime(I2C1, DS3231_I2C_ADDR, &dt);
    ds3231_set_alarm1_daily(I2C1, DS3231_I2C_ADDR, 12u, 0u, 0u);
    int16_t temp;
    ds3231_read_temperature(I2C1, DS3231_I2C_ADDR, &temp);
}

