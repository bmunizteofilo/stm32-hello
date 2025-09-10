#ifndef AR1021_H
#define AR1021_H

#include <stdbool.h>
#include <stdint.h>
#include "spi.h"
#include "gpio.h"

typedef uint16_t (*ar1021_transfer_f)(uint8_t cmd);

bool ar1021_init(ar1021_transfer_f transfer);
bool ar1021_read(uint16_t *x, uint16_t *y);
void ar1021_set_calibration(int16_t x_min, int16_t x_max,
                            int16_t y_min, int16_t y_max);
bool ar1021_init_spi(SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin);
void ar1021_example(void);

#endif /* AR1021_H */
