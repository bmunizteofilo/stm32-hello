#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"
#include "i2c.h"
#include "RealTime/rtc_utils.h"

#define DS1307_I2C_ADDR 0x68u

enum ds1307_sqwave {
    DS1307_SQW_OFF,
    DS1307_SQW_1HZ,
    DS1307_SQW_4KHZ,
    DS1307_SQW_8KHZ,
    DS1307_SQW_32KHZ,
};

bool ds1307_init(I2C_TypeDef *i2c, uint8_t addr);
bool ds1307_set_datetime(I2C_TypeDef *i2c, uint8_t addr, const rtc_datetime_t *dt);
bool ds1307_get_datetime(I2C_TypeDef *i2c, uint8_t addr, rtc_datetime_t *dt);
bool ds1307_set_squarewave(I2C_TypeDef *i2c, uint8_t addr, enum ds1307_sqwave mode);

void ds1307_example(void);

#endif /* DS1307_H */
