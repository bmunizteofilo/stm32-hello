#include "RealTime/ds1307.h"
#include <string.h>

static bool ds1307_write(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) {
    uint8_t buf[8];
    if (len + 1u > sizeof(buf)) return false;
    buf[0] = reg;
    for (size_t i = 0u; i < len; ++i) {
        buf[i + 1u] = data[i];
    }
    return i2c_write_poll(i2c, addr, buf, len + 1u);
}

static bool ds1307_read(I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data, size_t len) {
    if (!i2c_write_poll(i2c, addr, &reg, 1u)) return false;
    return i2c_read_poll(i2c, addr, data, len);
}

bool ds1307_init(I2C_TypeDef *i2c, uint8_t addr) {
    uint8_t sec;
    if (!ds1307_read(i2c, addr, 0x00u, &sec, 1u)) return false;
    sec &= 0x7Fu; /* clear CH bit */
    return ds1307_write(i2c, addr, 0x00u, &sec, 1u);
}

bool ds1307_set_datetime(I2C_TypeDef *i2c, uint8_t addr, const rtc_datetime_t *dt) {
    if (!dt) return false;
    uint8_t buf[7];
    buf[0] = rtc_bin2bcd(dt->seconds) & 0x7Fu;
    buf[1] = rtc_bin2bcd(dt->minutes);
    buf[2] = rtc_bin2bcd(dt->hours);
    buf[3] = rtc_bin2bcd(1u); /* day of week placeholder */
    buf[4] = rtc_bin2bcd(dt->day);
    buf[5] = rtc_bin2bcd(dt->month);
    buf[6] = rtc_bin2bcd((uint8_t)(dt->year % 100u));
    return ds1307_write(i2c, addr, 0x00u, buf, 7u);
}

bool ds1307_get_datetime(I2C_TypeDef *i2c, uint8_t addr, rtc_datetime_t *dt) {
    if (!dt) return false;
    uint8_t buf[7];
    if (!ds1307_read(i2c, addr, 0x00u, buf, 7u)) return false;
    dt->seconds = rtc_bcd2bin(buf[0] & 0x7Fu);
    dt->minutes = rtc_bcd2bin(buf[1]);
    dt->hours   = rtc_bcd2bin(buf[2] & 0x3Fu);
    dt->day     = rtc_bcd2bin(buf[4]);
    dt->month   = rtc_bcd2bin(buf[5]);
    dt->year    = (uint16_t)(2000u + rtc_bcd2bin(buf[6]));
    return true;
}

bool ds1307_set_squarewave(I2C_TypeDef *i2c, uint8_t addr, enum ds1307_sqwave mode) {
    uint8_t ctrl;
    switch (mode) {
        case DS1307_SQW_OFF:   ctrl = 0x00u; break;
        case DS1307_SQW_1HZ:   ctrl = 0x10u; break;
        case DS1307_SQW_4KHZ:  ctrl = 0x11u; break;
        case DS1307_SQW_8KHZ:  ctrl = 0x12u; break;
        case DS1307_SQW_32KHZ: ctrl = 0x13u; break;
        default: return false;
    }
    return ds1307_write(i2c, addr, 0x07u, &ctrl, 1u);
}

void ds1307_example(void) {
    rtc_datetime_t dt = { .year = 2024, .month = 1, .day = 1, .hours = 0, .minutes = 0, .seconds = 0 };
    ds1307_init(I2C1, DS1307_I2C_ADDR);
    ds1307_set_datetime(I2C1, DS1307_I2C_ADDR, &dt);
    ds1307_set_squarewave(I2C1, DS1307_I2C_ADDR, DS1307_SQW_1HZ);
}

