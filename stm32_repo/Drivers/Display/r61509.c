#include "r61509.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define R61509_SPI      SPI1
#define R61509_CS_PORT  GPIOA
#define R61509_CS_PIN   0u
#define R61509_DC_PORT  GPIOA
#define R61509_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define R61509_DB_PORT  GPIOB
#define R61509_WR_PORT  GPIOA
#define R61509_WR_PIN   2u
#define R61509_RD_PORT  GPIOA
#define R61509_RD_PIN   3u

static void r61509_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(R61509_DC_PORT, R61509_DC_PIN, dc);
    spi_cs_select(R61509_CS_PORT, R61509_CS_PIN, true);
    spi_transfer(R61509_SPI, value);
    spi_cs_select(R61509_CS_PORT, R61509_CS_PIN, false);
}

static void r61509_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(R61509_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(R61509_DC_PORT, R61509_DC_PIN, dc);
    gpio_write(R61509_CS_PORT, R61509_CS_PIN, 0u);
    gpio_write(R61509_WR_PORT, R61509_WR_PIN, 0u);
    gpio_write(R61509_WR_PORT, R61509_WR_PIN, 1u);
    gpio_write(R61509_CS_PORT, R61509_CS_PIN, 1u);
}

static void r61509_set_window(uint16_t x, uint16_t y) {
    r61509_write_spi(0u, 0x2Au);
    r61509_write_spi(1u, x >> 8);
    r61509_write_spi(1u, x & 0xFFu);
    r61509_write_spi(0u, 0x2Bu);
    r61509_write_spi(1u, y >> 8);
    r61509_write_spi(1u, y & 0xFFu);
    r61509_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void r61509_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(R61509_SPI, &cfg);
    spi_cs_init(R61509_CS_PORT, R61509_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(R61509_DC_PORT, R61509_DC_PIN, &pin);
    spi_enable(R61509_SPI, true);

    r61509_write_spi(0u, 0x01u); /* Software reset */
    r61509_write_spi(0u, 0x28u); /* Display OFF */
    r61509_write_spi(0u, 0x3Au);
    r61509_write_spi(1u, 0x55u); /* 16-bit color */
    r61509_write_spi(0u, 0x36u);
    r61509_write_spi(1u, 0x48u); /* Memory access control */
    r61509_write_spi(0u, 0xB4u); /* Controller specific */
    r61509_write_spi(0u, 0x11u); /* Sleep out */
    r61509_write_spi(0u, 0x29u); /* Display ON */
}

void r61509_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(R61509_DB_PORT, i, &out);
    }
    gpio_config(R61509_CS_PORT, R61509_CS_PIN, &out);
    gpio_config(R61509_DC_PORT, R61509_DC_PIN, &out);
    gpio_config(R61509_WR_PORT, R61509_WR_PIN, &out);
    gpio_config(R61509_RD_PORT, R61509_RD_PIN, &out);

    r61509_write_parallel(0u, 0x01u);
    r61509_write_parallel(0u, 0x28u);
    r61509_write_parallel(0u, 0x3Au);
    r61509_write_parallel(1u, 0x55u);
    r61509_write_parallel(0u, 0x36u);
    r61509_write_parallel(1u, 0x48u);
    r61509_write_parallel(0u, 0xB4u);
    r61509_write_parallel(0u, 0x11u);
    r61509_write_parallel(0u, 0x29u);
}

void r61509_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    r61509_set_window(x, y);
    r61509_write_spi(1u, color >> 8);
    r61509_write_spi(1u, color & 0xFFu);
}

void r61509_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            r61509_draw_pixel(x + col, y + row, color);
        }
    }
}

void r61509_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            r61509_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

