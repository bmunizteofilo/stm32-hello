#include "ra8875.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define RA8875_SPI      SPI1
#define RA8875_CS_PORT  GPIOA
#define RA8875_CS_PIN   0u
#define RA8875_DC_PORT  GPIOA
#define RA8875_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define RA8875_DB_PORT  GPIOB
#define RA8875_WR_PORT  GPIOA
#define RA8875_WR_PIN   2u
#define RA8875_RD_PORT  GPIOA
#define RA8875_RD_PIN   3u

static void ra8875_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(RA8875_DC_PORT, RA8875_DC_PIN, dc);
    spi_cs_select(RA8875_CS_PORT, RA8875_CS_PIN, true);
    spi_transfer(RA8875_SPI, value);
    spi_cs_select(RA8875_CS_PORT, RA8875_CS_PIN, false);
}

static void ra8875_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(RA8875_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(RA8875_DC_PORT, RA8875_DC_PIN, dc);
    gpio_write(RA8875_CS_PORT, RA8875_CS_PIN, 0u);
    gpio_write(RA8875_WR_PORT, RA8875_WR_PIN, 0u);
    gpio_write(RA8875_WR_PORT, RA8875_WR_PIN, 1u);
    gpio_write(RA8875_CS_PORT, RA8875_CS_PIN, 1u);
}

static void ra8875_set_window(uint16_t x, uint16_t y) {
    ra8875_write_spi(0u, 0x2Au);
    ra8875_write_spi(1u, x >> 8);
    ra8875_write_spi(1u, x & 0xFFu);
    ra8875_write_spi(0u, 0x2Bu);
    ra8875_write_spi(1u, y >> 8);
    ra8875_write_spi(1u, y & 0xFFu);
    ra8875_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void ra8875_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(RA8875_SPI, &cfg);
    spi_cs_init(RA8875_CS_PORT, RA8875_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(RA8875_DC_PORT, RA8875_DC_PIN, &pin);
    spi_enable(RA8875_SPI, true);

    ra8875_write_spi(0u, 0x01u); /* Software reset */
    ra8875_write_spi(0u, 0x28u); /* Display OFF */
    ra8875_write_spi(0u, 0x3Au);
    ra8875_write_spi(1u, 0x55u); /* 16-bit color */
    ra8875_write_spi(0u, 0x36u);
    ra8875_write_spi(1u, 0x48u); /* Memory access control */
    ra8875_write_spi(0u, 0xB0u); /* Controller specific */
    ra8875_write_spi(0u, 0x11u); /* Sleep out */
    ra8875_write_spi(0u, 0x29u); /* Display ON */
}

void ra8875_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(RA8875_DB_PORT, i, &out);
    }
    gpio_config(RA8875_CS_PORT, RA8875_CS_PIN, &out);
    gpio_config(RA8875_DC_PORT, RA8875_DC_PIN, &out);
    gpio_config(RA8875_WR_PORT, RA8875_WR_PIN, &out);
    gpio_config(RA8875_RD_PORT, RA8875_RD_PIN, &out);

    ra8875_write_parallel(0u, 0x01u);
    ra8875_write_parallel(0u, 0x28u);
    ra8875_write_parallel(0u, 0x3Au);
    ra8875_write_parallel(1u, 0x55u);
    ra8875_write_parallel(0u, 0x36u);
    ra8875_write_parallel(1u, 0x48u);
    ra8875_write_parallel(0u, 0xB0u);
    ra8875_write_parallel(0u, 0x11u);
    ra8875_write_parallel(0u, 0x29u);
}

void ra8875_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    ra8875_set_window(x, y);
    ra8875_write_spi(1u, color >> 8);
    ra8875_write_spi(1u, color & 0xFFu);
}

void ra8875_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ra8875_draw_pixel(x + col, y + row, color);
        }
    }
}

void ra8875_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ra8875_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

