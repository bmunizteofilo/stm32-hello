#ifndef CY8CTMA1036_H
#define CY8CTMA1036_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef bool (*cy8ctma1036_read_f)(uint8_t reg, uint8_t *buf, uint8_t len);
typedef bool (*cy8ctma1036_write_f)(uint8_t reg, const uint8_t *buf, uint8_t len);

bool cy8ctma1036_init(cy8ctma1036_read_f read_f,
                      cy8ctma1036_write_f write_f);
bool cy8ctma1036_read(uint16_t *x, uint16_t *y);
bool cy8ctma1036_init_i2c(I2C_TypeDef *i2c);
void cy8ctma1036_example(void);

#endif /* CY8CTMA1036_H */
