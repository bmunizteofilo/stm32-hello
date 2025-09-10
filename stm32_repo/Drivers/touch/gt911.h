#ifndef GT911_H
#define GT911_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef bool (*gt911_read_f)(uint16_t reg, uint8_t *buf, uint16_t len);
typedef bool (*gt911_write_f)(uint16_t reg, const uint8_t *buf, uint16_t len);

bool gt911_init(gt911_read_f read_f, gt911_write_f write_f);
bool gt911_read(uint16_t *x, uint16_t *y);
bool gt911_init_i2c(I2C_TypeDef *i2c);
void gt911_example(void);

#endif /* GT911_H */
