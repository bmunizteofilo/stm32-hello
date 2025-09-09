#include "s6d1121.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define S6D1121_SPI      SPI1
#define S6D1121_CS_PORT  GPIOA
#define S6D1121_CS_PIN   0u
#define S6D1121_DC_PORT  GPIOA
#define S6D1121_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define S6D1121_DB_PORT  GPIOB
#define S6D1121_WR_PORT  GPIOA
#define S6D1121_WR_PIN   2u
#define S6D1121_RD_PORT  GPIOA
#define S6D1121_RD_PIN   3u

static void s6d1121_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(S6D1121_DC_PORT, S6D1121_DC_PIN, dc);
    spi_cs_select(S6D1121_CS_PORT, S6D1121_CS_PIN, true);
    spi_transfer(S6D1121_SPI, value);
    spi_cs_select(S6D1121_CS_PORT, S6D1121_CS_PIN, false);
}

static void s6d1121_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(S6D1121_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(S6D1121_DC_PORT, S6D1121_DC_PIN, dc);
    gpio_write(S6D1121_CS_PORT, S6D1121_CS_PIN, 0u);
    gpio_write(S6D1121_WR_PORT, S6D1121_WR_PIN, 0u);
    gpio_write(S6D1121_WR_PORT, S6D1121_WR_PIN, 1u);
    gpio_write(S6D1121_CS_PORT, S6D1121_CS_PIN, 1u);
}

static void s6d1121_set_window(uint16_t x, uint16_t y) {
    s6d1121_write_spi(0u, 0x2Au);
    s6d1121_write_spi(1u, x >> 8);
    s6d1121_write_spi(1u, x & 0xFFu);
    s6d1121_write_spi(0u, 0x2Bu);
    s6d1121_write_spi(1u, y >> 8);
    s6d1121_write_spi(1u, y & 0xFFu);
    s6d1121_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void s6d1121_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(S6D1121_SPI, &cfg);
    spi_cs_init(S6D1121_CS_PORT, S6D1121_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(S6D1121_DC_PORT, S6D1121_DC_PIN, &pin);
    spi_enable(S6D1121_SPI, true);

    s6d1121_write_spi(0u, 0x01u); /* Software reset */
    s6d1121_write_spi(0u, 0x28u); /* Display OFF */
    s6d1121_write_spi(0u, 0x3Au);
    s6d1121_write_spi(1u, 0x55u); /* 16-bit color */
    s6d1121_write_spi(0u, 0x36u);
    s6d1121_write_spi(1u, 0x48u); /* Memory access control */
    s6d1121_write_spi(0u, 0xE0u); /* Controller specific */
    s6d1121_write_spi(0u, 0x11u); /* Sleep out */
    s6d1121_write_spi(0u, 0x29u); /* Display ON */
}

void s6d1121_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(S6D1121_DB_PORT, i, &out);
    }
    gpio_config(S6D1121_CS_PORT, S6D1121_CS_PIN, &out);
    gpio_config(S6D1121_DC_PORT, S6D1121_DC_PIN, &out);
    gpio_config(S6D1121_WR_PORT, S6D1121_WR_PIN, &out);
    gpio_config(S6D1121_RD_PORT, S6D1121_RD_PIN, &out);

    s6d1121_write_parallel(0u, 0x01u);
    s6d1121_write_parallel(0u, 0x28u);
    s6d1121_write_parallel(0u, 0x3Au);
    s6d1121_write_parallel(1u, 0x55u);
    s6d1121_write_parallel(0u, 0x36u);
    s6d1121_write_parallel(1u, 0x48u);
    s6d1121_write_parallel(0u, 0xE0u);
    s6d1121_write_parallel(0u, 0x11u);
    s6d1121_write_parallel(0u, 0x29u);
}

void s6d1121_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    s6d1121_set_window(x, y);
    s6d1121_write_spi(1u, color >> 8);
    s6d1121_write_spi(1u, color & 0xFFu);
}

void s6d1121_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            s6d1121_draw_pixel(x + col, y + row, color);
        }
    }
}

void s6d1121_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            s6d1121_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

