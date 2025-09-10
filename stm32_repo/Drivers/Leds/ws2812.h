#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>
#include <stddef.h>
#include "tim.h"
#include "dma.h"

/** \file ws2812.h
 *  \brief Driver for WS2812 (NeoPixel) RGB LEDs.
 *
 *  The WS2812 LED is controlled by a single-wire protocol at 800 kHz.
 *  This driver prepares a PWM buffer with timing values suitable to
 *  drive the LEDs using a timer + DMA setup.
 *
 *  Example of usage:
 *  @code
 *  ws2812_color_t leds[8];
 *  uint16_t buffer[WS2812_BUFFER_LEN(8)];
 *  ws2812_t strip;
 *  ws2812_init(&strip, 8, leds, buffer);
 *  // Configure timer for 800 kHz bitstream (48 MHz clock -> period 59)
 *  tim_cfg_t tcfg = { .mode = TIM_COUNTER_UP, .preload = true,
 *                     .prescaler = 0,
 *                     .period = WS2812_TIMER_PERIOD(48000000u) };
 *  tim_init(TIM3, &tcfg);
 *  const ws2812_color_t colors[3] = {
 *      {255,0,0}, {0,255,0}, {0,0,255}
 *  };
 *  for (int frame = 0; frame < 3; ++frame) {
 *      for (int i = 0; i < 8; ++i) {
 *          ws2812_color_t c = colors[(frame + i)%3];
 *          ws2812_set_pixel(&strip, i, c.r, c.g, c.b);
 *      }
 *      ws2812_prepare_buffer(&strip);
 *      ws2812_send_tim_dma(&strip, TIM3, 1u, &DMA1->CH[0]);
 *  }
 *  uint32_t us = ws2812_estimate_transfer_time_us(&strip);
 *  (void)us;
 *  @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Representation of an RGB color for WS2812. */
typedef struct {
    uint8_t r; /**< Red component. */
    uint8_t g; /**< Green component. */
    uint8_t b; /**< Blue component. */
} ws2812_color_t;

/** WS2812 driver handle. */
typedef struct {
    uint16_t length;          /**< Number of LEDs in the strip. */
    ws2812_color_t *pixels;   /**< Array of color values. */
    uint16_t *buffer;         /**< PWM buffer to be filled by the driver. */
} ws2812_t;

/** Bit rate of the WS2812 protocol. */
#define WS2812_BIT_HZ 800000u

/** Compute timer period for a given timer input clock. */
#define WS2812_TIMER_PERIOD(timer_clk_hz) (((timer_clk_hz) / WS2812_BIT_HZ) - 1u)

/**
 * Number of timer ticks a logical '0' stays high.
 * Values assume timer period of 59 (60 ticks total).
 */
#define WS2812_CODE_0 19u
/** Number of timer ticks a logical '1' stays high for the same period. */
#define WS2812_CODE_1 38u

/** Compute required buffer size for @p n LEDs. */
#define WS2812_BUFFER_LEN(n) ((n) * 24u)

/**
 * @brief Initialize a WS2812 strip instance.
 * @param dev      Driver handle.
 * @param length   Number of LEDs.
 * @param pixels   Array of @ref ws2812_color_t with at least @p length elements.
 * @param buffer   Output buffer with size of @ref WS2812_BUFFER_LEN.
 */
void ws2812_init(ws2812_t *dev, uint16_t length, ws2812_color_t *pixels, uint16_t *buffer);

/**
 * @brief Set the color of a single pixel.
 * @param dev   Driver handle.
 * @param index LED index (0-based).
 * @param r     Red value.
 * @param g     Green value.
 * @param b     Blue value.
 */
void ws2812_set_pixel(ws2812_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Convert pixel data into PWM buffer.
 * @param dev Driver handle.
 */
void ws2812_prepare_buffer(ws2812_t *dev);

/**
 * @brief Start DMA transfer of the PWM buffer using a timer channel.
 * @param dev   Driver handle with buffer prepared.
 * @param tim   Timer generating the PWM signal.
 * @param ch    Timer channel (1..4).
 * @param dma_ch DMA channel configured for the timer CCR register.
 */
void ws2812_send_tim_dma(const ws2812_t *dev, TIM_TypeDef *tim, uint8_t ch, DMA_Channel_TypeDef *dma_ch);

/**
 * @brief Estimate total transmission time in microseconds.
 *        Each WS2812 LED requires 30 µs (24 bits at 800 kHz).
 * @param dev Driver handle.
 * @return Time in microseconds to send the buffer.
 */
uint32_t ws2812_estimate_transfer_time_us(const ws2812_t *dev);

/**
 * @brief Example function performing a simple color chase.
 */
void ws2812_example_sequence(void);

#ifdef __cplusplus
}
#endif

#endif /* WS2812_H */
