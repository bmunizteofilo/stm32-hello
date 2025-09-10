#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>
#include "gpio.h"

typedef struct {
    GPIO_TypeDef *clk_port;
    uint8_t clk_pin;
    GPIO_TypeDef *dio_port;
    uint8_t dio_pin;
    uint8_t brightness; /* 0..7 */
} tm1637_t;

void tm1637_init(tm1637_t *dev, GPIO_TypeDef *clk_port, uint8_t clk_pin,
                 GPIO_TypeDef *dio_port, uint8_t dio_pin);
void tm1637_set_brightness(tm1637_t *dev, uint8_t level);
void tm1637_display_segments(tm1637_t *dev, const uint8_t segments[4]);
void tm1637_display_digits(tm1637_t *dev, const uint8_t digits[4]);
void tm1637_display_number(tm1637_t *dev, int value);
void tm1637_clear(tm1637_t *dev);

#endif /* TM1637_H */
