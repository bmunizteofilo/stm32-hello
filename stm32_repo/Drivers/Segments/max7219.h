#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>
#include "spi.h"
#include "gpio.h"

typedef struct {
    SPI_TypeDef *spi;
    GPIO_TypeDef *cs_port;
    uint8_t cs_pin;
} max7219_t;

void max7219_init(max7219_t *dev, SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin);
void max7219_set_intensity(const max7219_t *dev, uint8_t intensity);
void max7219_display_digit(const max7219_t *dev, uint8_t digit, uint8_t value);
void max7219_display_number(const max7219_t *dev, int32_t value);
void max7219_clear(const max7219_t *dev);

#endif /* MAX7219_H */
