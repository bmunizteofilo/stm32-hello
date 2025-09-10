#include "ws2812.h"
#include "tim.h"
#include "dma.h"

void ws2812_init(ws2812_t *dev, uint16_t length, ws2812_color_t *pixels, uint16_t *buffer) {
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
    }
}

void ws2812_set_pixel(ws2812_t *dev, uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!dev || index >= dev->length) {
        return;
    }
    dev->pixels[index].r = r;
    dev->pixels[index].g = g;
    dev->pixels[index].b = b;
}

void ws2812_prepare_buffer(ws2812_t *dev) {
    if (!dev) {
        return;
    }
    uint16_t *buf = dev->buffer;
    for (uint16_t i = 0; i < dev->length; ++i) {
        ws2812_color_t c = dev->pixels[i];
        uint8_t colors[3] = {c.g, c.r, c.b}; /* WS2812 expects GRB order */
        for (unsigned j = 0; j < 3; ++j) {
            uint8_t mask = 0x80u;
            while (mask) {
                *buf++ = (colors[j] & mask) ? WS2812_CODE_1 : WS2812_CODE_0;
                mask >>= 1;
            }
        }
    }
}

void ws2812_send_tim_dma(const ws2812_t *dev, TIM_TypeDef *tim, uint8_t ch, DMA_Channel_TypeDef *dma_ch) {
    if (!dev || !tim || !dma_ch || ch == 0u || ch > 4u) {
        return;
    }
    volatile uint32_t *ccr = tim_ccr_ptr(tim, ch);
    uint32_t ccr_cfg = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE |
                       (1u << DMA_CCR_PSIZE_SHIFT) | (1u << DMA_CCR_MSIZE_SHIFT);
    dma_config_channel(dma_ch, ccr_cfg);
    dma_set_peripheral(dma_ch, (const void *)ccr);
    dma_set_memory(dma_ch, dev->buffer);
    dma_set_count(dma_ch, WS2812_BUFFER_LEN(dev->length));
    dma_enable(dma_ch, true);
    tim_enable_dma(tim, TIM_DIER_CC1DE << (ch - 1u), true);
}

uint32_t ws2812_estimate_transfer_time_us(const ws2812_t *dev) {
    return dev ? (dev->length * 30u) : 0u;
}

/**
 * @brief Example: color chase on a 4-LED WS2812 strip.
 *
 * Demonstrates init, buffer preparation and DMA transmission using TIM3 ch1.
 */
void ws2812_example_sequence(void) {
    ws2812_color_t pixels[4];
    uint16_t buffer[WS2812_BUFFER_LEN(4)];
    ws2812_t strip;
    ws2812_init(&strip, 4, pixels, buffer);
    const ws2812_color_t palette[3] = {
        {255u, 0u, 0u},
        {0u, 255u, 0u},
        {0u, 0u, 255u},
    };
    for (size_t step = 0; step < 3; ++step) {
        for (uint16_t i = 0; i < strip.length; ++i) {
            ws2812_color_t c = palette[(step + i) % 3u];
            ws2812_set_pixel(&strip, i, c.r, c.g, c.b);
        }
        ws2812_prepare_buffer(&strip);
        ws2812_send_tim_dma(&strip, TIM3, 1u, &DMA1->CH[0]);
    }
}
