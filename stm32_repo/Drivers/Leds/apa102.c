#include "apa102.h"
#include "spi.h"

void apa102_init(apa102_t *dev, uint16_t length, apa102_color_t *pixels, uint8_t *buffer) {
    if (!dev) {
        return;
    }
    dev->length = length;
    dev->pixels = pixels;
    dev->buffer = buffer;
    dev->global_brightness = 31u; /* max by default */
    for (uint16_t i = 0; i < length; ++i) {
        dev->pixels[i].r = 0;
        dev->pixels[i].g = 0;
        dev->pixels[i].b = 0;
    }
}

void apa102_set_pixel(apa102_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!dev || index >= dev->length) {
        return;
    }
    dev->pixels[index].r = r;
    dev->pixels[index].g = g;
    dev->pixels[index].b = b;
}

void apa102_set_global_brightness(apa102_t *dev, uint8_t brightness) {
    if (!dev) {
        return;
    }
    dev->global_brightness = brightness & 0x1Fu;
}

void apa102_prepare_buffer(apa102_t *dev) {
    if (!dev) {
        return;
    }
    uint8_t *buf = dev->buffer;
    /* Start frame */
    for (int i = 0; i < 4; ++i) {
        *buf++ = 0x00;
    }
    /* LED frames */
    for (uint16_t i = 0; i < dev->length; ++i) {
        apa102_color_t c = dev->pixels[i];
        *buf++ = 0xE0u | dev->global_brightness;
        *buf++ = c.b;
        *buf++ = c.g;
        *buf++ = c.r;
    }
    /* End frame */
    for (int i = 0; i < 4; ++i) {
        *buf++ = 0xFFu;
    }
}

void apa102_send_spi(const apa102_t *dev, SPI_TypeDef *spi) {
    if (!dev || !spi) {
        return;
    }
    uint32_t len = APA102_BUFFER_LEN(dev->length);
    for (uint32_t i = 0; i < len; ++i) {
        spi_transfer(spi, dev->buffer[i]);
    }
}

void apa102_send_spi_dma(const apa102_t *dev, SPI_TypeDef *spi, DMA_Channel_TypeDef *tx_ch) {
    if (!dev || !spi || !tx_ch) {
        return;
    }
    uint32_t len = APA102_BUFFER_LEN(dev->length);
    /* Transmit buffer using DMA; RX path unused. */
    spi_transfer_dma_start(spi, tx_ch, NULL, (const uint16_t *)dev->buffer, NULL, len, NULL, NULL);
}

uint32_t apa102_estimate_transfer_time_us(const apa102_t *dev, uint32_t spi_freq_hz) {
    if (!dev || spi_freq_hz == 0u) {
        return 0u;
    }
    uint32_t bits = APA102_BUFFER_LEN(dev->length) * 8u;
    return (bits * 1000000u + spi_freq_hz - 1u) / spi_freq_hz;
}

/**
 * @brief Example: cycle colors on a 4-LED APA102 strip using SPI.
 *
 * The example initializes SPI1, prepares a small strip and sends
 * several frames cycling through red, green, blue and white.
 */
void apa102_example_sequence(void) {
    apa102_color_t pixels[4];
    uint8_t buffer[APA102_BUFFER_LEN(4)];
    apa102_t strip;
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
        .baudrate = SPI_BAUD_DIV8,
    };
    spi_init(SPI1, &cfg);
    spi_enable(SPI1, true);
    apa102_init(&strip, 4, pixels, buffer);
    const apa102_color_t palette[4] = {
        {255u, 0u, 0u},
        {0u, 255u, 0u},
        {0u, 0u, 255u},
        {255u, 255u, 255u},
    };
    for (size_t step = 0; step < 4; ++step) {
        for (uint16_t i = 0; i < strip.length; ++i) {
            apa102_color_t c = palette[(step + i) % 4u];
            apa102_set_pixel(&strip, i, c.r, c.g, c.b);
        }
        apa102_prepare_buffer(&strip);
        apa102_send_spi(&strip, SPI1);
    }
}
