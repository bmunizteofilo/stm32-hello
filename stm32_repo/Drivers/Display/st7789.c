#include "st7789.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define ST7789_SPI      SPI1
#define ST7789_CS_PORT  GPIOA
#define ST7789_CS_PIN   0u
#define ST7789_DC_PORT  GPIOA
#define ST7789_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define ST7789_DB_PORT  GPIOB
#define ST7789_WR_PORT  GPIOA
#define ST7789_WR_PIN   2u
#define ST7789_RD_PORT  GPIOA
#define ST7789_RD_PIN   3u

static void st7789_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(ST7789_DC_PORT, ST7789_DC_PIN, dc);
    spi_cs_select(ST7789_CS_PORT, ST7789_CS_PIN, true);
    spi_transfer(ST7789_SPI, value);
    spi_cs_select(ST7789_CS_PORT, ST7789_CS_PIN, false);
}

static void st7789_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(ST7789_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(ST7789_DC_PORT, ST7789_DC_PIN, dc);
    gpio_write(ST7789_CS_PORT, ST7789_CS_PIN, 0u);
    gpio_write(ST7789_WR_PORT, ST7789_WR_PIN, 0u);
    gpio_write(ST7789_WR_PORT, ST7789_WR_PIN, 1u);
    gpio_write(ST7789_CS_PORT, ST7789_CS_PIN, 1u);
}

static void st7789_set_window(uint16_t x, uint16_t y) {
    st7789_write_spi(0u, 0x2Au);
    st7789_write_spi(1u, x >> 8);
    st7789_write_spi(1u, x & 0xFFu);
    st7789_write_spi(0u, 0x2Bu);
    st7789_write_spi(1u, y >> 8);
    st7789_write_spi(1u, y & 0xFFu);
    st7789_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void st7789_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(ST7789_SPI, &cfg);
    spi_cs_init(ST7789_CS_PORT, ST7789_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(ST7789_DC_PORT, ST7789_DC_PIN, &pin);
    spi_enable(ST7789_SPI, true);

    st7789_write_spi(0u, 0x01u); /* Software reset */
    st7789_write_spi(0u, 0x28u); /* Display OFF */
    st7789_write_spi(0u, 0x3Au);
    st7789_write_spi(1u, 0x55u); /* 16-bit color */
    st7789_write_spi(0u, 0x36u);
    st7789_write_spi(1u, 0x48u); /* Memory access control */
    st7789_write_spi(0u, 0xB2u); /* Controller specific */
    st7789_write_spi(0u, 0x11u); /* Sleep out */
    st7789_write_spi(0u, 0x29u); /* Display ON */
}

void st7789_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(ST7789_DB_PORT, i, &out);
    }
    gpio_config(ST7789_CS_PORT, ST7789_CS_PIN, &out);
    gpio_config(ST7789_DC_PORT, ST7789_DC_PIN, &out);
    gpio_config(ST7789_WR_PORT, ST7789_WR_PIN, &out);
    gpio_config(ST7789_RD_PORT, ST7789_RD_PIN, &out);

    st7789_write_parallel(0u, 0x01u);
    st7789_write_parallel(0u, 0x28u);
    st7789_write_parallel(0u, 0x3Au);
    st7789_write_parallel(1u, 0x55u);
    st7789_write_parallel(0u, 0x36u);
    st7789_write_parallel(1u, 0x48u);
    st7789_write_parallel(0u, 0xB2u);
    st7789_write_parallel(0u, 0x11u);
    st7789_write_parallel(0u, 0x29u);
}

void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    st7789_set_window(x, y);
    st7789_write_spi(1u, color >> 8);
    st7789_write_spi(1u, color & 0xFFu);
}

void st7789_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            st7789_draw_pixel(x + col, y + row, color);
        }
    }
}

void st7789_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            st7789_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

