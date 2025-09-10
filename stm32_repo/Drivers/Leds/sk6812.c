#include "sk6812.h"
#include "tim.h"
#include "dma.h"

void sk6812_init(sk6812_t *dev, uint16_t length, sk6812_color_t *pixels, uint16_t *buffer) {
    if (!dev) {
        return;
    }
    dev->length = length;
    dev->pixels = pixels;
    dev->buffer = buffer;
    for (uint16_t i = 0; i < length; ++i) {
        dev->pixels[i].r = 0;
        dev->pixels[i].g = 0;
        dev->pixels[i].b = 0;
        dev->pixels[i].w = 0;
    }
}

void sk6812_set_pixel(sk6812_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    if (!dev || index >= dev->length) {
        return;
    }
    dev->pixels[index].r = r;
    dev->pixels[index].g = g;
    dev->pixels[index].b = b;
    dev->pixels[index].w = w;
}

void sk6812_prepare_buffer(sk6812_t *dev) {
    if (!dev) {
        return;
    }
    uint16_t *buf = dev->buffer;
    for (uint16_t i = 0; i < dev->length; ++i) {
        sk6812_color_t c = dev->pixels[i];
        uint8_t colors[4] = {c.g, c.r, c.b, c.w}; /* SK6812 expects GRBW */
        for (unsigned j = 0; j < 4; ++j) {
            uint8_t mask = 0x80u;
            while (mask) {
                *buf++ = (colors[j] & mask) ? SK6812_CODE_1 : SK6812_CODE_0;
                mask >>= 1;
            }
        }
    }
}

void sk6812_send_tim_dma(const sk6812_t *dev, TIM_TypeDef *tim, uint8_t ch, DMA_Channel_TypeDef *dma_ch) {
    if (!dev || !tim || !dma_ch || ch == 0u || ch > 4u) {
        return;
    }
    volatile uint32_t *ccr = tim_ccr_ptr(tim, ch);
    uint32_t ccr_cfg = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE |
                       (1u << DMA_CCR_PSIZE_SHIFT) | (1u << DMA_CCR_MSIZE_SHIFT);
    dma_config_channel(dma_ch, ccr_cfg);
    dma_set_peripheral(dma_ch, (const void *)ccr);
    dma_set_memory(dma_ch, dev->buffer);
    dma_set_count(dma_ch, SK6812_BUFFER_LEN(dev->length));
    dma_enable(dma_ch, true);
    tim_enable_dma(tim, TIM_DIER_CC1DE << (ch - 1u), true);
}

uint32_t sk6812_estimate_transfer_time_us(const sk6812_t *dev) {
    return dev ? (dev->length * 40u) : 0u;
}

/**
 * @brief Example: cycle RGBW colors on a 4-LED SK6812 strip.
 *
 * Uses TIM3 channel 2 for PWM + DMA transmission.
 */
void sk6812_example_sequence(void) {
    sk6812_color_t pixels[4];
    uint16_t buffer[SK6812_BUFFER_LEN(4)];
    sk6812_t strip;
    sk6812_init(&strip, 4, pixels, buffer);
    const sk6812_color_t palette[4] = {
        {255u, 0u, 0u, 0u},
        {0u, 255u, 0u, 0u},
        {0u, 0u, 255u, 0u},
        {0u, 0u, 0u, 255u},
    };
    for (size_t step = 0; step < 4; ++step) {
        for (uint16_t i = 0; i < strip.length; ++i) {
            sk6812_color_t c = palette[(step + i) % 4u];
            sk6812_set_pixel(&strip, i, c.r, c.g, c.b, c.w);
        }
        sk6812_prepare_buffer(&strip);
        sk6812_send_tim_dma(&strip, TIM3, 2u, &DMA1->CH[1]);
    }
}
