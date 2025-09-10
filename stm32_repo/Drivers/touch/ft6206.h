#ifndef FT6206_H
#define FT6206_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef bool (*ft6206_read_f)(uint8_t reg, uint8_t *buf, uint8_t len);
typedef bool (*ft6206_write_f)(uint8_t reg, const uint8_t *buf, uint8_t len);

bool ft6206_init(ft6206_read_f read_f, ft6206_write_f write_f);
bool ft6206_read(uint16_t *x, uint16_t *y);
bool ft6206_init_i2c(I2C_TypeDef *i2c);
void ft6206_example(void);

#endif /* FT6206_H */
