#ifndef DS3231_H
#define DS3231_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"
#include "i2c.h"
#include "RealTime/rtc_utils.h"

#define DS3231_I2C_ADDR 0x68u

bool ds3231_set_datetime(I2C_TypeDef *i2c, uint8_t addr, const rtc_datetime_t *dt);
bool ds3231_get_datetime(I2C_TypeDef *i2c, uint8_t addr, rtc_datetime_t *dt);
bool ds3231_read_temperature(I2C_TypeDef *i2c, uint8_t addr, int16_t *temp_c_256);
bool ds3231_set_alarm1_daily(I2C_TypeDef *i2c, uint8_t addr, uint8_t hour, uint8_t minute, uint8_t second);

void ds3231_example(void);

#endif /* DS3231_H */
