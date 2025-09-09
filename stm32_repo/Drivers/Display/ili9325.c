#include "ili9325.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define ILI9325_SPI      SPI1
#define ILI9325_CS_PORT  GPIOA
#define ILI9325_CS_PIN   0u
#define ILI9325_DC_PORT  GPIOA
#define ILI9325_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define ILI9325_DB_PORT  GPIOB
#define ILI9325_WR_PORT  GPIOA
#define ILI9325_WR_PIN   2u
#define ILI9325_RD_PORT  GPIOA
#define ILI9325_RD_PIN   3u

static void ili9325_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(ILI9325_DC_PORT, ILI9325_DC_PIN, dc);
    spi_cs_select(ILI9325_CS_PORT, ILI9325_CS_PIN, true);
    spi_transfer(ILI9325_SPI, value);
    spi_cs_select(ILI9325_CS_PORT, ILI9325_CS_PIN, false);
}

static void ili9325_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(ILI9325_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(ILI9325_DC_PORT, ILI9325_DC_PIN, dc);
    gpio_write(ILI9325_CS_PORT, ILI9325_CS_PIN, 0u);
    gpio_write(ILI9325_WR_PORT, ILI9325_WR_PIN, 0u);
    gpio_write(ILI9325_WR_PORT, ILI9325_WR_PIN, 1u);
    gpio_write(ILI9325_CS_PORT, ILI9325_CS_PIN, 1u);
}

static void ili9325_set_window(uint16_t x, uint16_t y) {
    ili9325_write_spi(0u, 0x2Au);
    ili9325_write_spi(1u, x >> 8);
    ili9325_write_spi(1u, x & 0xFFu);
    ili9325_write_spi(0u, 0x2Bu);
    ili9325_write_spi(1u, y >> 8);
    ili9325_write_spi(1u, y & 0xFFu);
    ili9325_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void ili9325_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(ILI9325_SPI, &cfg);
    spi_cs_init(ILI9325_CS_PORT, ILI9325_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(ILI9325_DC_PORT, ILI9325_DC_PIN, &pin);
    spi_enable(ILI9325_SPI, true);

    ili9325_write_spi(0u, 0x01u); /* Software reset */
    ili9325_write_spi(0u, 0x28u); /* Display OFF */
    ili9325_write_spi(0u, 0x3Au);
    ili9325_write_spi(1u, 0x55u); /* 16-bit color */
    ili9325_write_spi(0u, 0x36u);
    ili9325_write_spi(1u, 0x48u); /* Memory access control */
    ili9325_write_spi(0u, 0xE5u); /* Controller specific */
    ili9325_write_spi(0u, 0x11u); /* Sleep out */
    ili9325_write_spi(0u, 0x29u); /* Display ON */
}

void ili9325_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(ILI9325_DB_PORT, i, &out);
    }
    gpio_config(ILI9325_CS_PORT, ILI9325_CS_PIN, &out);
    gpio_config(ILI9325_DC_PORT, ILI9325_DC_PIN, &out);
    gpio_config(ILI9325_WR_PORT, ILI9325_WR_PIN, &out);
    gpio_config(ILI9325_RD_PORT, ILI9325_RD_PIN, &out);

    ili9325_write_parallel(0u, 0x01u);
    ili9325_write_parallel(0u, 0x28u);
    ili9325_write_parallel(0u, 0x3Au);
    ili9325_write_parallel(1u, 0x55u);
    ili9325_write_parallel(0u, 0x36u);
    ili9325_write_parallel(1u, 0x48u);
    ili9325_write_parallel(0u, 0xE5u);
    ili9325_write_parallel(0u, 0x11u);
    ili9325_write_parallel(0u, 0x29u);
}

void ili9325_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    ili9325_set_window(x, y);
    ili9325_write_spi(1u, color >> 8);
    ili9325_write_spi(1u, color & 0xFFu);
}

void ili9325_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ili9325_draw_pixel(x + col, y + row, color);
        }
    }
}

void ili9325_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ili9325_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

