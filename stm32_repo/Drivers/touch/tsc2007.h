#ifndef TSC2007_H
#define TSC2007_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef uint16_t (*tsc2007_read_f)(uint8_t cmd);

bool tsc2007_init(tsc2007_read_f reader);
bool tsc2007_read(uint16_t *x, uint16_t *y);
void tsc2007_set_calibration(int16_t x_min, int16_t x_max,
                             int16_t y_min, int16_t y_max);
bool tsc2007_init_i2c(I2C_TypeDef *i2c);
void tsc2007_example(void);

#endif /* TSC2007_H */
