#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stdbool.h>

#define DMA1_BASE 0x40020000u

typedef struct {
    volatile uint32_t CCR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
    volatile uint32_t RESERVED;
} DMA_Channel_TypeDef;

typedef struct {
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    DMA_Channel_TypeDef CH[7];
} DMA_TypeDef;

#define DMA1 ((DMA_TypeDef *)DMA1_BASE)

#define DMA_CCR_EN      (1u << 0)
#define DMA_CCR_TCIE    (1u << 1)
#define DMA_CCR_HTIE    (1u << 2)
#define DMA_CCR_TEIE    (1u << 3)
#define DMA_CCR_DIR     (1u << 4)
#define DMA_CCR_CIRC    (1u << 5)
#define DMA_CCR_PINC    (1u << 6)
#define DMA_CCR_MINC    (1u << 7)
#define DMA_CCR_PSIZE_SHIFT 8
#define DMA_CCR_MSIZE_SHIFT 10
#define DMA_CCR_PL_SHIFT    12
#define DMA_CCR_MEM2MEM (1u << 14)

#define DMA_IRQ_TC 0x1u
#define DMA_IRQ_HT 0x2u
#define DMA_IRQ_TE 0x4u

typedef void (*dma_cb_t)(void *ctx, uint32_t flags);

bool dma_config_channel(DMA_Channel_TypeDef *ch, uint32_t ccr);
void dma_set_peripheral(DMA_Channel_TypeDef *ch, const void *addr);
void dma_set_memory(DMA_Channel_TypeDef *ch, void *addr);
void dma_set_count(DMA_Channel_TypeDef *ch, uint16_t count);
void dma_enable(DMA_Channel_TypeDef *ch, bool en);
void dma_set_callback(DMA_TypeDef *dma, uint8_t ch_idx, dma_cb_t cb, void *ctx);
uint32_t dma_get_isr(DMA_TypeDef *dma, uint8_t ch_idx);
void dma_clear_isr(DMA_TypeDef *dma, uint8_t ch_idx, uint32_t flags);

#endif /* DMA_H */
