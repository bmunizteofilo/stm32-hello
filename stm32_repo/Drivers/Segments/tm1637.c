#include <stdbool.h>
#include "Segments/tm1637.h"

static void tm1637_delay(void) {
    for (volatile int i = 0; i < 100; ++i) { (void)i; }
}

static void tm1637_write_bit(tm1637_t *dev, uint8_t bit) {
    gpio_write(dev->clk_port, dev->clk_pin, 0u);
    tm1637_delay();
    gpio_write(dev->dio_port, dev->dio_pin, bit);
    tm1637_delay();
    gpio_write(dev->clk_port, dev->clk_pin, 1u);
    tm1637_delay();
}

static void tm1637_start(tm1637_t *dev) {
    gpio_write(dev->dio_port, dev->dio_pin, 1u);
    gpio_write(dev->clk_port, dev->clk_pin, 1u);
    tm1637_delay();
    gpio_write(dev->dio_port, dev->dio_pin, 0u);
}

static void tm1637_stop(tm1637_t *dev) {
    gpio_write(dev->clk_port, dev->clk_pin, 0u);
    tm1637_delay();
    gpio_write(dev->dio_port, dev->dio_pin, 0u);
    tm1637_delay();
    gpio_write(dev->clk_port, dev->clk_pin, 1u);
    tm1637_delay();
    gpio_write(dev->dio_port, dev->dio_pin, 1u);
}

static void tm1637_ack(tm1637_t *dev) {
    gpio_write(dev->clk_port, dev->clk_pin, 0u);
    gpio_write(dev->dio_port, dev->dio_pin, 1u);
    tm1637_delay();
    gpio_write(dev->clk_port, dev->clk_pin, 1u);
    tm1637_delay();
    gpio_write(dev->clk_port, dev->clk_pin, 0u);
}

static void tm1637_write_byte(tm1637_t *dev, uint8_t byte) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        tm1637_write_bit(dev, byte & 0x01u);
        byte >>= 1;
    }
    tm1637_ack(dev);
}

void tm1637_init(tm1637_t *dev, GPIO_TypeDef *clk_port, uint8_t clk_pin,
                 GPIO_TypeDef *dio_port, uint8_t dio_pin) {
    if (!dev) {
        return;
    }
    dev->clk_port = clk_port;
    dev->clk_pin = clk_pin;
    dev->dio_port = dio_port;
    dev->dio_pin = dio_pin;
    dev->brightness = 7u;

    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_OD,
        .pull = GPIO_PULL_UP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(clk_port, clk_pin, &out);
    gpio_config(dio_port, dio_pin, &out);
    gpio_write(clk_port, clk_pin, 1u);
    gpio_write(dio_port, dio_pin, 1u);
}

void tm1637_set_brightness(tm1637_t *dev, uint8_t level) {
    if (!dev) {
        return;
    }
    dev->brightness = level & 0x07u;
    tm1637_start(dev);
    tm1637_write_byte(dev, 0x88u | dev->brightness);
    tm1637_stop(dev);
}

static const uint8_t tm1637_digit_map[17] = {
    0x3Fu,0x06u,0x5Bu,0x4Fu,0x66u,0x6Du,0x7Du,0x07u,
    0x7Fu,0x6Fu,0x40u,0x77u,0x7Cu,0x39u,0x5Eu,0x79u,0x00u
};

void tm1637_display_segments(tm1637_t *dev, const uint8_t segments[4]) {
    if (!dev || !segments) {
        return;
    }
    tm1637_start(dev);
    tm1637_write_byte(dev, 0x40u);
    tm1637_stop(dev);
    tm1637_start(dev);
    tm1637_write_byte(dev, 0xC0u);
    for (uint8_t i = 0u; i < 4u; ++i) {
        tm1637_write_byte(dev, segments[i]);
    }
    tm1637_stop(dev);
    tm1637_start(dev);
    tm1637_write_byte(dev, 0x88u | dev->brightness);
    tm1637_stop(dev);
}

void tm1637_display_digits(tm1637_t *dev, const uint8_t digits[4]) {
    if (!dev || !digits) {
        return;
    }
    uint8_t seg[4];
    for (uint8_t i = 0u; i < 4u; ++i) {
        uint8_t d = digits[i];
        if (d < 16u) {
            seg[i] = tm1637_digit_map[d];
        } else {
            seg[i] = tm1637_digit_map[16];
        }
    }
    tm1637_display_segments(dev, seg);
}

void tm1637_display_number(tm1637_t *dev, int value) {
    if (!dev) {
        return;
    }
    bool neg = false;
    if (value < 0) {
        neg = true;
        value = -value;
    }
    uint8_t digits[4];
    for (uint8_t i = 0u; i < 4u; ++i) {
        if (value > 0 || i == 0u) {
            digits[i] = (uint8_t)(value % 10);
            value /= 10;
        } else if (neg) {
            digits[i] = 10u; /* minus sign */
            neg = false;
        } else {
            digits[i] = 16u; /* blank */
        }
    }
    tm1637_display_digits(dev, digits);
}

void tm1637_clear(tm1637_t *dev) {
    if (!dev) {
        return;
    }
    uint8_t off[4] = {0u,0u,0u,0u};
    tm1637_display_segments(dev, off);
}
