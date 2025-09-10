#ifndef XPT2046_H
#define XPT2046_H

#include <stdbool.h>
#include <stdint.h>
#include "spi.h"
#include "gpio.h"

typedef uint16_t (*xpt2046_transfer_f)(uint8_t cmd);

bool xpt2046_init(xpt2046_transfer_f transfer);
bool xpt2046_read(uint16_t *x, uint16_t *y);
void xpt2046_set_calibration(int16_t x_min, int16_t x_max,
                             int16_t y_min, int16_t y_max);
bool xpt2046_init_spi(SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin);
void xpt2046_example(void);

#endif /* XPT2046_H */
