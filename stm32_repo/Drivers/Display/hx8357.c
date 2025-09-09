#include "hx8357.h"
#include "spi.h"
#include "gpio.h"

/* Default pin mapping for SPI interface */
#define HX8357_SPI      SPI1
#define HX8357_CS_PORT  GPIOA
#define HX8357_CS_PIN   0u
#define HX8357_DC_PORT  GPIOA
#define HX8357_DC_PIN   1u

/* Parallel interface control pins (8-bit data bus on GPIOB[0..7]) */
#define HX8357_DB_PORT  GPIOB
#define HX8357_WR_PORT  GPIOA
#define HX8357_WR_PIN   2u
#define HX8357_RD_PORT  GPIOA
#define HX8357_RD_PIN   3u

static void hx8357_write_spi(uint8_t dc, uint8_t value) {
    gpio_write(HX8357_DC_PORT, HX8357_DC_PIN, dc);
    spi_cs_select(HX8357_CS_PORT, HX8357_CS_PIN, true);
    spi_transfer(HX8357_SPI, value);
    spi_cs_select(HX8357_CS_PORT, HX8357_CS_PIN, false);
}

static void hx8357_write_parallel(uint8_t dc, uint8_t value) {
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_write(HX8357_DB_PORT, i, (value >> i) & 1u);
    }
    gpio_write(HX8357_DC_PORT, HX8357_DC_PIN, dc);
    gpio_write(HX8357_CS_PORT, HX8357_CS_PIN, 0u);
    gpio_write(HX8357_WR_PORT, HX8357_WR_PIN, 0u);
    gpio_write(HX8357_WR_PORT, HX8357_WR_PIN, 1u);
    gpio_write(HX8357_CS_PORT, HX8357_CS_PIN, 1u);
}

static void hx8357_set_window(uint16_t x, uint16_t y) {
    hx8357_write_spi(0u, 0x2Au);
    hx8357_write_spi(1u, x >> 8);
    hx8357_write_spi(1u, x & 0xFFu);
    hx8357_write_spi(0u, 0x2Bu);
    hx8357_write_spi(1u, y >> 8);
    hx8357_write_spi(1u, y & 0xFFu);
    hx8357_write_spi(0u, 0x2Cu);
}

/* Simplified init sequence; consult datasheet for full details. */
void hx8357_init_spi(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(HX8357_SPI, &cfg);
    spi_cs_init(HX8357_CS_PORT, HX8357_CS_PIN);
    gpio_cfg_t pin = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    gpio_config(HX8357_DC_PORT, HX8357_DC_PIN, &pin);
    spi_enable(HX8357_SPI, true);

    hx8357_write_spi(0u, 0x01u); /* Software reset */
    hx8357_write_spi(0u, 0x28u); /* Display OFF */
    hx8357_write_spi(0u, 0x3Au);
    hx8357_write_spi(1u, 0x55u); /* 16-bit color */
    hx8357_write_spi(0u, 0x36u);
    hx8357_write_spi(1u, 0x48u); /* Memory access control */
    hx8357_write_spi(0u, 0xB1u); /* Controller specific */
    hx8357_write_spi(0u, 0x11u); /* Sleep out */
    hx8357_write_spi(0u, 0x29u); /* Display ON */
}

void hx8357_init_parallel(void) {
    gpio_cfg_t out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NONE,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    };
    for (uint8_t i = 0u; i < 8u; ++i) {
        gpio_config(HX8357_DB_PORT, i, &out);
    }
    gpio_config(HX8357_CS_PORT, HX8357_CS_PIN, &out);
    gpio_config(HX8357_DC_PORT, HX8357_DC_PIN, &out);
    gpio_config(HX8357_WR_PORT, HX8357_WR_PIN, &out);
    gpio_config(HX8357_RD_PORT, HX8357_RD_PIN, &out);

    hx8357_write_parallel(0u, 0x01u);
    hx8357_write_parallel(0u, 0x28u);
    hx8357_write_parallel(0u, 0x3Au);
    hx8357_write_parallel(1u, 0x55u);
    hx8357_write_parallel(0u, 0x36u);
    hx8357_write_parallel(1u, 0x48u);
    hx8357_write_parallel(0u, 0xB1u);
    hx8357_write_parallel(0u, 0x11u);
    hx8357_write_parallel(0u, 0x29u);
}

void hx8357_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    hx8357_set_window(x, y);
    hx8357_write_spi(1u, color >> 8);
    hx8357_write_spi(1u, color & 0xFFu);
}

void hx8357_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            hx8357_draw_pixel(x + col, y + row, color);
        }
    }
}

void hx8357_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h) {
    for (uint16_t row = 0u; row < h; ++row) {
        for (uint16_t col = 0u; col < w; ++col) {
            hx8357_draw_pixel(x + col, y + row, data[row * w + col]);
        }
    }
}

