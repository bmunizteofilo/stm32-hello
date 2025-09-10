#ifndef STMPE811_H
#define STMPE811_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef bool (*stmpe811_read_f)(uint8_t reg, uint8_t *buf, uint8_t len);
typedef bool (*stmpe811_write_f)(uint8_t reg, const uint8_t *buf, uint8_t len);

bool stmpe811_init(stmpe811_read_f read_f, stmpe811_write_f write_f);
bool stmpe811_read(uint16_t *x, uint16_t *y);
bool stmpe811_init_i2c(I2C_TypeDef *i2c);
void stmpe811_example(void);

#endif /* STMPE811_H */
