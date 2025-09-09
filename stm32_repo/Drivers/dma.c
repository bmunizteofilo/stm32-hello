#include "dma.h"
#include "cm0.h"

typedef struct { dma_cb_t cb; void *ctx; } dma_cb_entry_t;
static dma_cb_entry_t dma_callbacks[7];

bool dma_config_channel(DMA_Channel_TypeDef *ch, uint32_t ccr) {
    if (!ch) return false;
    ch->CCR = ccr;
    return true;
}

void dma_set_peripheral(DMA_Channel_TypeDef *ch, const void *addr) {
    ch->CPAR = (uint32_t)(uintptr_t)addr;
}

void dma_set_memory(DMA_Channel_TypeDef *ch, void *addr) {
    ch->CMAR = (uint32_t)(uintptr_t)addr;
}

void dma_set_count(DMA_Channel_TypeDef *ch, uint16_t count) {
    ch->CNDTR = count;
}

void dma_enable(DMA_Channel_TypeDef *ch, bool en) {
    if (en) ch->CCR |= DMA_CCR_EN; else ch->CCR &= ~DMA_CCR_EN;
}

void dma_set_callback(DMA_TypeDef *dma, uint8_t ch_idx, dma_cb_t cb, void *ctx) {
    if (!dma || ch_idx == 0u || ch_idx > 7u) return;
    dma_callbacks[ch_idx - 1u].cb = cb;
    dma_callbacks[ch_idx - 1u].ctx = ctx;
    if (cb) {
        /* enable NVIC channel; numbers 11..17 for DMA1 Ch1..Ch7 */
        cm0_nvic_enable((IRQn_Type)(11 + (ch_idx - 1u)));
    } else {
        cm0_nvic_disable((IRQn_Type)(11 + (ch_idx - 1u)));
    }
}

uint32_t dma_get_isr(DMA_TypeDef *dma, uint8_t ch_idx) {
    uint32_t shift = (ch_idx - 1u) * 4u;
    return (dma->ISR >> shift) & 0xFu;
}

void dma_clear_isr(DMA_TypeDef *dma, uint8_t ch_idx, uint32_t flags) {
    uint32_t shift = (ch_idx - 1u) * 4u;
    uint32_t mask = 0u;
    if (flags & DMA_IRQ_TC) mask |= (1u << (1 + shift));
    if (flags & DMA_IRQ_HT) mask |= (1u << (2 + shift));
    if (flags & DMA_IRQ_TE) mask |= (1u << (3 + shift));
    dma->IFCR = mask;
}

static void dma_dispatch(uint8_t ch_idx) {
    dma_cb_entry_t *e = &dma_callbacks[ch_idx - 1u];
    if (e->cb) {
        uint32_t isr = dma_get_isr(DMA1, ch_idx);
        uint32_t flags = 0u;
        if (isr & (1u << 1)) flags |= DMA_IRQ_TC;
        if (isr & (1u << 2)) flags |= DMA_IRQ_HT;
        if (isr & (1u << 3)) flags |= DMA_IRQ_TE;
        dma_clear_isr(DMA1, ch_idx, DMA_IRQ_TC | DMA_IRQ_HT | DMA_IRQ_TE);
        e->cb(e->ctx, flags);
    }
}

void DMA1_Channel1_IRQHandler(void) { dma_dispatch(1u); }
void DMA1_Channel2_3_IRQHandler(void) { dma_dispatch(2u); dma_dispatch(3u); }
void DMA1_Channel4_5_IRQHandler(void) { dma_dispatch(4u); dma_dispatch(5u); }
void DMA1_Channel6_7_IRQHandler(void) { dma_dispatch(6u); dma_dispatch(7u); }

