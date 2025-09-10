#ifndef SK6812_H
#define SK6812_H

#include <stdint.h>
#include <stddef.h>
#include "tim.h"
#include "dma.h"

/** \file sk6812.h
 *  \brief Driver for SK6812 RGBW LEDs.
 *
 *  The SK6812 extends the WS2812 with an additional white channel. The
 *  protocol also runs at 800 kHz. This driver converts color data into a
 *  PWM buffer suitable for transmission via timer + DMA.
 *
 *  Example:
 *  @code
 *  sk6812_color_t leds[5];
 *  uint16_t buffer[SK6812_BUFFER_LEN(5)];
 *  sk6812_t strip;
 *  sk6812_init(&strip, 5, leds, buffer);
 *  // Timer clock 48 MHz -> period 59 for 800 kHz
 *  tim_cfg_t tcfg = { .mode = TIM_COUNTER_UP, .preload = true,
 *                     .prescaler = 0,
 *                     .period = SK6812_TIMER_PERIOD(48000000u) };
 *  tim_init(TIM3, &tcfg);
 *  const sk6812_color_t pal[4] = {
 *      {255,0,0,0}, {0,255,0,0}, {0,0,255,0}, {0,0,0,255}
 *  };
 *  for (int f = 0; f < 4; ++f) {
 *      for (int i = 0; i < 5; ++i) {
 *          sk6812_color_t c = pal[(f+i)%4];
 *          sk6812_set_pixel(&strip, i, c.r, c.g, c.b, c.w);
 *      }
 *      sk6812_prepare_buffer(&strip);
 *      sk6812_send_tim_dma(&strip, TIM3, 2u, &DMA1->CH[1]);
 *  }
 *  uint32_t us = sk6812_estimate_transfer_time_us(&strip);
 *  (void)us;
 *  @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

/** SK6812 color representation (RGBW). */
typedef struct {
    uint8_t r; /**< Red component. */
    uint8_t g; /**< Green component. */
    uint8_t b; /**< Blue component. */
    uint8_t w; /**< White component. */
} sk6812_color_t;

/** SK6812 driver handle. */
typedef struct {
    uint16_t length;            /**< Number of LEDs. */
    sk6812_color_t *pixels;     /**< Color array. */
    uint16_t *buffer;           /**< PWM buffer. */
} sk6812_t;

/** Bit rate of the SK6812 protocol. */
#define SK6812_BIT_HZ 800000u

/** Compute timer period for a given timer clock. */
#define SK6812_TIMER_PERIOD(timer_clk_hz) (((timer_clk_hz) / SK6812_BIT_HZ) - 1u)

/**
 * Timer ticks the signal remains high for a logic '0'.
 * Assumes timer period of 59 (60 ticks total).
 */
#define SK6812_CODE_0 19u
/** Timer ticks the signal remains high for a logic '1' under same period. */
#define SK6812_CODE_1 38u

/** Buffer size needed for @p n LEDs. */
#define SK6812_BUFFER_LEN(n) ((n) * 32u)

void sk6812_init(sk6812_t *dev, uint16_t length, sk6812_color_t *pixels, uint16_t *buffer);
void sk6812_set_pixel(sk6812_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void sk6812_prepare_buffer(sk6812_t *dev);

/**
 * @brief Start DMA transfer of the PWM buffer using a timer channel.
 * @param dev    Driver handle.
 * @param tim    Timer instance.
 * @param ch     Timer channel (1..4).
 * @param dma_ch DMA channel associated with the timer channel.
 */
void sk6812_send_tim_dma(const sk6812_t *dev, TIM_TypeDef *tim, uint8_t ch, DMA_Channel_TypeDef *dma_ch);

/**
 * @brief Estimate time in microseconds to transmit the buffer.
 *        Each SK6812 LED requires 40 µs (32 bits at 800 kHz).
 * @param dev Driver handle.
 * @return Transfer time in microseconds.
 */
uint32_t sk6812_estimate_transfer_time_us(const sk6812_t *dev);

/**
 * @brief Example function cycling RGBW colors.
 */
void sk6812_example_sequence(void);

#ifdef __cplusplus
}
#endif

#endif /* SK6812_H */
