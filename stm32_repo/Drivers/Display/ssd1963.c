#include "ssd1963.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define SSD1963_SPI      SPI1
#define SSD1963_CS_PORT  GPIOA
#define SSD1963_CS_PIN   0u
#define SSD1963_DC_PORT  GPIOA
#define SSD1963_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define SSD1963_DB_PORT  GPIOB
#define SSD1963_WR_PORT  GPIOA
#define SSD1963_WR_PIN   2u
#define SSD1963_RD_PORT  GPIOA
#define SSD1963_RD_PIN   3u

static void ssd1963_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(SSD1963_DC_PORT, SSD1963_DC_PIN, dc);
    spi_cs_select(SSD1963_CS_PORT, SSD1963_CS_PIN, true);
    spi_transfer(SSD1963_SPI, value);
    spi_cs_select(SSD1963_CS_PORT, SSD1963_CS_PIN, false);
}

static void ssd1963_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(SSD1963_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(SSD1963_DC_PORT, SSD1963_DC_PIN, dc);
    gpio_write(SSD1963_CS_PORT, SSD1963_CS_PIN, 0u);
    gpio_write(SSD1963_WR_PORT, SSD1963_WR_PIN, 0u);
    gpio_write(SSD1963_WR_PORT, SSD1963_WR_PIN, 1u);
    gpio_write(SSD1963_CS_PORT, SSD1963_CS_PIN, 1u);
}

static void ssd1963_set_window(uint16_t x, uint16_t y) {
    ssd1963_write_spi(0u, 0x2Au);
    ssd1963_write_spi(1u, x >> 8);
    ssd1963_write_spi(1u, x & 0xFFu);
    ssd1963_write_spi(0u, 0x2Bu);
    ssd1963_write_spi(1u, y >> 8);
    ssd1963_write_spi(1u, y & 0xFFu);
    ssd1963_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void ssd1963_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(SSD1963_SPI, &cfg);
    spi_cs_init(SSD1963_CS_PORT, SSD1963_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(SSD1963_DC_PORT, SSD1963_DC_PIN, &pin);
    spi_enable(SSD1963_SPI, true);

    ssd1963_write_spi(0u, 0x01u); /* Software reset */
    ssd1963_write_spi(0u, 0x28u); /* Display OFF */
    ssd1963_write_spi(0u, 0x3Au);
    ssd1963_write_spi(1u, 0x55u); /* 16-bit color */
    ssd1963_write_spi(0u, 0x36u);
    ssd1963_write_spi(1u, 0x48u); /* Memory access control */
    ssd1963_write_spi(0u, 0xE2u); /* Controller specific */
    ssd1963_write_spi(0u, 0x11u); /* Sleep out */
    ssd1963_write_spi(0u, 0x29u); /* Display ON */
}

void ssd1963_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(SSD1963_DB_PORT, i, &out);
    }
    gpio_config(SSD1963_CS_PORT, SSD1963_CS_PIN, &out);
    gpio_config(SSD1963_DC_PORT, SSD1963_DC_PIN, &out);
    gpio_config(SSD1963_WR_PORT, SSD1963_WR_PIN, &out);
    gpio_config(SSD1963_RD_PORT, SSD1963_RD_PIN, &out);

    ssd1963_write_parallel(0u, 0x01u);
    ssd1963_write_parallel(0u, 0x28u);
    ssd1963_write_parallel(0u, 0x3Au);
    ssd1963_write_parallel(1u, 0x55u);
    ssd1963_write_parallel(0u, 0x36u);
    ssd1963_write_parallel(1u, 0x48u);
    ssd1963_write_parallel(0u, 0xE2u);
    ssd1963_write_parallel(0u, 0x11u);
    ssd1963_write_parallel(0u, 0x29u);
}

void ssd1963_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    ssd1963_set_window(x, y);
    ssd1963_write_spi(1u, color >> 8);
    ssd1963_write_spi(1u, color & 0xFFu);
}

void ssd1963_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ssd1963_draw_pixel(x + col, y + row, color);
        }
    }
}

void ssd1963_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            ssd1963_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

