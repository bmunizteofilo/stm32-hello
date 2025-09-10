#ifndef ADS7843_H
#define ADS7843_H

#include <stdbool.h>
#include <stdint.h>
#include "spi.h"
#include "gpio.h"

typedef uint16_t (*ads7843_transfer_f)(uint8_t cmd);

bool ads7843_init(ads7843_transfer_f transfer);
bool ads7843_read(uint16_t *x, uint16_t *y);
void ads7843_set_calibration(int16_t x_min, int16_t x_max,
                             int16_t y_min, int16_t y_max);
bool ads7843_init_spi(SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin);
void ads7843_example(void);

#endif /* ADS7843_H */
