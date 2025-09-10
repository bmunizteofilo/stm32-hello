#include <stdbool.h>
#include "Segments/max7219.h"

#define MAX7219_REG_NOOP        0x00u
#define MAX7219_REG_DIGIT0      0x01u
#define MAX7219_REG_DECODE_MODE 0x09u
#define MAX7219_REG_INTENSITY   0x0Au
#define MAX7219_REG_SCAN_LIMIT  0x0Bu
#define MAX7219_REG_SHUTDOWN    0x0Cu

static void max7219_send(const max7219_t *dev, uint8_t addr, uint8_t data) {
    if (!dev || !dev->spi || !dev->cs_port) {
        return;
    }
    spi_cs_select(dev->cs_port, dev->cs_pin, true);
    spi_transfer(dev->spi, addr);
    spi_transfer(dev->spi, data);
    spi_cs_select(dev->cs_port, dev->cs_pin, false);
}

void max7219_init(max7219_t *dev, SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin) {
    if (!dev) {
        return;
    }
    dev->spi = spi;
    dev->cs_port = cs_port;
    dev->cs_pin = cs_pin;

    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
        .baudrate = SPI_BAUD_DIV16,
    };
    spi_init(spi, &cfg);
    spi_cs_init(cs_port, cs_pin);
    spi_enable(spi, true);

    max7219_send(dev, MAX7219_REG_DECODE_MODE, 0xFFu);
    max7219_send(dev, MAX7219_REG_SCAN_LIMIT, 0x07u);
    max7219_send(dev, MAX7219_REG_SHUTDOWN, 0x01u);
    max7219_set_intensity(dev, 8u);
    max7219_clear(dev);
}

void max7219_set_intensity(const max7219_t *dev, uint8_t intensity) {
    if (!dev) {
        return;
    }
    max7219_send(dev, MAX7219_REG_INTENSITY, intensity & 0x0Fu);
}

void max7219_display_digit(const max7219_t *dev, uint8_t digit, uint8_t value) {
    if (!dev || digit >= 8u) {
        return;
    }
    max7219_send(dev, MAX7219_REG_DIGIT0 + digit, value);
}

void max7219_display_number(const max7219_t *dev, int32_t value) {
    if (!dev) {
        return;
    }
    bool neg = false;
    if (value < 0) {
        neg = true;
        value = -value;
    }
    for (uint8_t i = 0u; i < 8u; ++i) {
        if (value > 0 || i == 0u) {
            max7219_display_digit(dev, i, (uint8_t)(value % 10));
            value /= 10;
        } else if (neg) {
            max7219_display_digit(dev, i, 0x0Au); /* minus */
            neg = false;
        } else {
            max7219_display_digit(dev, i, 0x0Fu); /* blank */
        }
    }
}

void max7219_clear(const max7219_t *dev) {
    if (!dev) {
        return;
    }
    for (uint8_t i = 0u; i < 8u; ++i) {
        max7219_display_digit(dev, i, 0x0Fu);
    }
}
