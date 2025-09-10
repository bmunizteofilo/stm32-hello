#ifndef FT5406_H
#define FT5406_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef bool (*ft5406_read_f)(uint8_t reg, uint8_t *buf, uint8_t len);
typedef bool (*ft5406_write_f)(uint8_t reg, const uint8_t *buf, uint8_t len);

bool ft5406_init(ft5406_read_f read_f, ft5406_write_f write_f);
bool ft5406_read(uint16_t *x, uint16_t *y);
bool ft5406_init_i2c(I2C_TypeDef *i2c);
void ft5406_example(void);

#endif /* FT5406_H */
