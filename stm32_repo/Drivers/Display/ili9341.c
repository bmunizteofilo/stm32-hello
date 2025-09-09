#include "ili9341.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define ILI9341_SPI      SPI1
#define ILI9341_CS_PORT  GPIOA
#define ILI9341_CS_PIN   0u
#define ILI9341_DC_PORT  GPIOA
#define ILI9341_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define ILI9341_DB_PORT  GPIOB
#define ILI9341_WR_PORT  GPIOA
#define ILI9341_WR_PIN   2u
#define ILI9341_RD_PORT  GPIOA
#define ILI9341_RD_PIN   3u

static void ili9341_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(ILI9341_DC_PORT, ILI9341_DC_PIN, dc);
    spi_cs_select(ILI9341_CS_PORT, ILI9341_CS_PIN, true);
    spi_transfer(ILI9341_SPI, value);
    spi_cs_select(ILI9341_CS_PORT, ILI9341_CS_PIN, false);
}

static void ili9341_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(ILI9341_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(ILI9341_DC_PORT, ILI9341_DC_PIN, dc);
    gpio_write(ILI9341_CS_PORT, ILI9341_CS_PIN, 0u);
    gpio_write(ILI9341_WR_PORT, ILI9341_WR_PIN, 0u);
    gpio_write(ILI9341_WR_PORT, ILI9341_WR_PIN, 1u);
    gpio_write(ILI9341_CS_PORT, ILI9341_CS_PIN, 1u);
}

static void ili9341_set_window(uint16_t x, uint16_t y) {
    ili9341_write_spi(0u, 0x2Au);
    ili9341_write_spi(1u, x >> 8);
    ili9341_write_spi(1u, x & 0xFFu);
    ili9341_write_spi(0u, 0x2Bu);
    ili9341_write_spi(1u, y >> 8);
    ili9341_write_spi(1u, y & 0xFFu);
    ili9341_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void ili9341_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(ILI9341_SPI, &cfg);
    spi_cs_init(ILI9341_CS_PORT, ILI9341_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(ILI9341_DC_PORT, ILI9341_DC_PIN, &pin);
    spi_enable(ILI9341_SPI, true);

    ili9341_write_spi(0u, 0x01u); /* Software reset */
    ili9341_write_spi(0u, 0x28u); /* Display OFF */
    ili9341_write_spi(0u, 0x3Au);
    ili9341_write_spi(1u, 0x55u); /* 16-bit color */
    ili9341_write_spi(0u, 0x36u);
    ili9341_write_spi(1u, 0x48u); /* Memory access control */
    ili9341_write_spi(0u, 0xCFu); /* Controller specific */
    ili9341_write_spi(0u, 0x11u); /* Sleep out */
    ili9341_write_spi(0u, 0x29u); /* Display ON */
}

void ili9341_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(ILI9341_DB_PORT, i, &out);
    }
    gpio_config(ILI9341_CS_PORT, ILI9341_CS_PIN, &out);
    gpio_config(ILI9341_DC_PORT, ILI9341_DC_PIN, &out);
    gpio_config(ILI9341_WR_PORT, ILI9341_WR_PIN, &out);
    gpio_config(ILI9341_RD_PORT, ILI9341_RD_PIN, &out);

    ili9341_write_parallel(0u, 0x01u);
    ili9341_write_parallel(0u, 0x28u);
    ili9341_write_parallel(0u, 0x3Au);
    ili9341_write_parallel(1u, 0x55u);
    ili9341_write_parallel(0u, 0x36u);
    ili9341_write_parallel(1u, 0x48u);
    ili9341_write_parallel(0u, 0xCFu);
    ili9341_write_parallel(0u, 0x11u);
    ili9341_write_parallel(0u, 0x29u);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    ili9341_set_window(x, y);
    ili9341_write_spi(1u, color >> 8);
    ili9341_write_spi(1u, color & 0xFFu);
}

void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ili9341_draw_pixel(x + col, y + row, color);
        }
    }
}

void ili9341_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ili9341_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

