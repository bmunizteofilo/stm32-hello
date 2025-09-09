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

/** Callback type for DMA transfer events. */
typedef void (*dma_cb_t)(void *ctx, uint32_t flags);

/**
 * @brief Configure a DMA channel control register.
 * @param ch  DMA channel instance.
 * @param ccr Value for the CCR register.
 * @return true on success, false on invalid arguments.
 */
bool dma_config_channel(DMA_Channel_TypeDef *ch, uint32_t ccr);

/**
 * @brief Set the peripheral address for a channel.
 * @param ch   DMA channel instance.
 * @param addr Peripheral source/destination address.
 */
void dma_set_peripheral(DMA_Channel_TypeDef *ch, const void *addr);

/**
 * @brief Set the memory address for a channel.
 * @param ch   DMA channel instance.
 * @param addr Memory source/destination address.
 */
void dma_set_memory(DMA_Channel_TypeDef *ch, void *addr);

/**
 * @brief Configure the number of transfers for a channel.
 * @param ch    DMA channel instance.
 * @param count Number of items to transfer.
 */
void dma_set_count(DMA_Channel_TypeDef *ch, uint16_t count);

/**
 * @brief Enable or disable a DMA channel.
 * @param ch DMA channel instance.
 * @param en true to enable, false to disable.
 */
void dma_enable(DMA_Channel_TypeDef *ch, bool en);

/**
 * @brief Register a callback for a DMA channel.
 * @param dma     DMA controller instance.
 * @param ch_idx  Channel index (1..7).
 * @param cb      Callback function.
 * @param ctx     User context passed to callback.
 */
void dma_set_callback(DMA_TypeDef *dma, uint8_t ch_idx, dma_cb_t cb, void *ctx);

/**
 * @brief Read interrupt status for a channel.
 * @param dma    DMA controller instance.
 * @param ch_idx Channel index.
 * @return Flags for the specified channel.
 */
uint32_t dma_get_isr(DMA_TypeDef *dma, uint8_t ch_idx);

/**
 * @brief Clear interrupt flags for a channel.
 * @param dma    DMA controller instance.
 * @param ch_idx Channel index.
 * @param flags  Flags to clear.
 */
void dma_clear_isr(DMA_TypeDef *dma, uint8_t ch_idx, uint32_t flags);

#endif /* DMA_H */
