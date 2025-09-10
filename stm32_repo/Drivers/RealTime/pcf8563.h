#ifndef PCF8563_H
#define PCF8563_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"
#include "i2c.h"
#include "RealTime/rtc_utils.h"

#define PCF8563_I2C_ADDR 0x51u

bool pcf8563_set_datetime(I2C_TypeDef *i2c, uint8_t addr, const rtc_datetime_t *dt);
bool pcf8563_get_datetime(I2C_TypeDef *i2c, uint8_t addr, rtc_datetime_t *dt);
bool pcf8563_set_alarm(I2C_TypeDef *i2c, uint8_t addr, uint8_t minute, uint8_t hour, uint8_t day);

void pcf8563_example(void);

#endif /* PCF8563_H */
