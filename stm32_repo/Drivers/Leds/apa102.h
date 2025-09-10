#ifndef APA102_H
#define APA102_H

#include <stdint.h>
#include <stddef.h>
#include "spi.h"

/** \file apa102.h
 *  \brief Driver for APA102 (DotStar) RGB LEDs using SPI.
 *
 *  The APA102 uses a SPI-like interface with start and end frames. This
 *  driver prepares a buffer that can be sent via SPI (optionally using
 *  DMA).
 *
 *  Example:
 *  @code
 *  apa102_color_t leds[10];
 *  uint8_t buffer[APA102_BUFFER_LEN(10)];
 *  apa102_t strip;
 *  spi_cfg_t cfg = {
 *      .mode = SPI_MODE_MASTER,
 *      .direction = SPI_DIR_FULL_DUPLEX,
 *      .cpol = SPI_CPOL_LOW,
 *      .cpha = SPI_CPHA_1EDGE,
 *      .datasize = 8u,
 *      .first_bit = SPI_FIRST_MSB,
 *      .nss = SPI_NSS_SOFT,
 *      .baudrate = SPI_BAUD_DIV8,
 *  };
 *  spi_init(SPI1, &cfg);
 *  spi_enable(SPI1, true);
 *  apa102_init(&strip, 10, leds, buffer);
 *  for (uint16_t i = 0; i < 10; ++i) {
 *      // Rastreia um ponto vermelho
 *      apa102_set_pixel(&strip, i, 0xFF, 0x00, 0x00);
 *      apa102_prepare_buffer(&strip);
 *      apa102_send_spi(&strip, SPI1);
 *      apa102_set_pixel(&strip, i, 0x00, 0x00, 0x00);
 *  }
 *  uint32_t us = apa102_estimate_transfer_time_us(&strip, 6000000u); // SPI ~6 MHz
 *  (void)us;
 *  @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/** APA102 color representation. */
typedef struct {
    uint8_t r; /**< Red component. */
    uint8_t g; /**< Green component. */
    uint8_t b; /**< Blue component. */
} apa102_color_t;

/** APA102 driver handle. */
typedef struct {
    uint16_t length;          /**< Number of LEDs. */
    uint8_t global_brightness;/**< Brightness (0-31). */
    apa102_color_t *pixels;   /**< Color array. */
    uint8_t *buffer;          /**< SPI buffer. */
} apa102_t;

/** Compute buffer length for @p n LEDs. */
#define APA102_BUFFER_LEN(n) (4u + ((n) * 4u) + 4u)

/** Initialize APA102 driver structure. */
void apa102_init(apa102_t *dev, uint16_t length, apa102_color_t *pixels, uint8_t *buffer);

/** Set pixel color. */
void apa102_set_pixel(apa102_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b);

/** Set global brightness (0-31). */
void apa102_set_global_brightness(apa102_t *dev, uint8_t brightness);

/** Prepare SPI buffer with LED data. */
void apa102_prepare_buffer(apa102_t *dev);

/**
 * Send prepared buffer via SPI.
 * The SPI clock defines the LED update speed; values up to 8 MHz are typical.
 */
void apa102_send_spi(const apa102_t *dev, SPI_TypeDef *spi);

/**
 * @brief Start DMA transfer of the prepared buffer over SPI.
 * @param dev   APA102 driver handle.
 * @param spi   SPI peripheral used for transmission.
 * @param tx_ch DMA channel connected to the SPI TX request.
 */
void apa102_send_spi_dma(const apa102_t *dev, SPI_TypeDef *spi, DMA_Channel_TypeDef *tx_ch);

/**
 * @brief Estimate transmission time for current buffer.
 * @param dev         APA102 driver handle.
 * @param spi_freq_hz SPI clock frequency in Hz.
 * @return Time in microseconds to transmit the buffer.
 */
uint32_t apa102_estimate_transfer_time_us(const apa102_t *dev, uint32_t spi_freq_hz);

/**
 * @brief Example function: cycle colors on a 4-LED strip.
 *
 * Useful as a quick reference in user applications.
 */
void apa102_example_sequence(void);

#ifdef __cplusplus
}
#endif

#endif /* APA102_H */
