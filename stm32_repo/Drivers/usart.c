/*
 * USART driver for STM32F0 without HAL.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "usart.h"
#include "rcc.h"
#include "cm0.h"

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
} USART_TypeDef_real;

/** Convert generic USART pointer to register structure. */
static inline USART_TypeDef_real *usart_real(USART_TypeDef *u) {
    return (USART_TypeDef_real *)u;
}

typedef struct {
    const uint8_t *tx;
    uint8_t *rx;
    size_t len;
    size_t tx_idx;
    size_t rx_idx;
    usart_cb_t cb;
    void *ctx;
} usart_it_state_t;

typedef struct {
    USART_TypeDef *usart;
    usart_cb_t cb;
    void *ctx;
    uint8_t pending;
} usart_dma_state_t;

typedef struct {
    usart_irq_cb_t cb;
    void *ctx;
} usart_irq_entry_t;

static usart_it_state_t usart_it_state[4];
static usart_dma_state_t usart_dma_state[4];
static usart_irq_entry_t usart_irq_table[4];

/** Translate USART instance to index. */
static uint32_t usart_index(USART_TypeDef *u) {
    if (u == USART1) return 0u;
    if (u == USART2) return 1u;
    if (u == USART3) return 2u;
    if (u == USART4) return 3u;
    return 4u;
}

/** Obtain DMA channel index (1-based). */
static uint8_t dma_channel_index(DMA_Channel_TypeDef *ch) {
    return (uint8_t)(ch - &DMA1->CH[0]) + 1u;
}

/** Initialize a USART peripheral. */
bool usart_init(USART_TypeDef *usart, const usart_cfg_t *cfg) {
    if (!usart || !cfg || cfg->baudrate == 0u) {
        return false;
    }
    USART_TypeDef_real *u = usart_real(usart);
    if (usart == USART1) {
        rcc_apb2_enable(RCC_APB2ENR_USART1);
    } else if (usart == USART2) {
        rcc_apb1_enable(RCC_APB1ENR_USART2);
    } else if (usart == USART3) {
        rcc_apb1_enable(RCC_APB1ENR_USART3);
    } else if (usart == USART4) {
        rcc_apb1_enable(RCC_APB1ENR_USART4);
    } else {
        return false;
    }
    u->CR1 = 0u;
    u->CR2 = 0u;
    u->CR3 = 0u;
    uint32_t cr1 = 0u;
    uint32_t cr2 = 0u;
    uint32_t cr3 = 0u;
    /* Word length */
    if (cfg->word_length == USART_WORD_9) {
        cr1 |= (1u << 12);
    }
    /* Parity */
    if (cfg->parity != USART_PARITY_NONE) {
        cr1 |= (1u << 10); /* PCE */
        if (cfg->parity == USART_PARITY_ODD) {
            cr1 |= (1u << 9); /* PS */
        }
    }
    /* Stop bits */
    switch (cfg->stop_bits) {
    case USART_STOP_0_5: cr2 |= (1u << 12); break;
    case USART_STOP_2:   cr2 |= (2u << 12); break;
    case USART_STOP_1_5: cr2 |= (3u << 12); break;
    default: break;
    }
    /* Mode */
    if (cfg->mode & USART_MODE_RX) cr1 |= (1u << 2);
    if (cfg->mode & USART_MODE_TX) cr1 |= (1u << 3);
    /* Hardware flow control */
    if (cfg->hw_flow == USART_HW_RTS || cfg->hw_flow == USART_HW_RTS_CTS) {
        cr3 |= (1u << 8);
    }
    if (cfg->hw_flow == USART_HW_CTS || cfg->hw_flow == USART_HW_RTS_CTS) {
        cr3 |= (1u << 9);
    }
    /* Oversampling */
    if (cfg->oversampling == USART_OVER8) {
        cr1 |= (1u << 15);
    }
    u->CR1 = cr1;
    u->CR2 = cr2;
    u->CR3 = cr3;
    uint32_t clk = rcc_sysclk_hz();
    if (clk == 0u) clk = 8000000u;
    u->BRR = clk / cfg->baudrate;
    return true;
}

/** Enable or disable a USART. */
void usart_enable(USART_TypeDef *usart, bool en) {
    if (!usart) return;
    USART_TypeDef_real *u = usart_real(usart);
    if (en) {
        u->CR1 |= (1u << 0);
    } else {
        u->CR1 &= ~(1u << 0);
    }
}

/** Perform blocking transmit. */
bool usart_write_poll(USART_TypeDef *usart, const uint8_t *data, size_t len) {
    if (!usart || (!data && len > 0u)) return false;
    USART_TypeDef_real *u = usart_real(usart);
    for (size_t i = 0u; i < len; ++i) {
        while ((u->ISR & (1u << 7)) == 0u) { }
        u->TDR = data[i];
    }
    return true;
}

/** Perform blocking receive. */
bool usart_read_poll(USART_TypeDef *usart, uint8_t *data, size_t len) {
    if (!usart || !data) return false;
    USART_TypeDef_real *u = usart_real(usart);
    for (size_t i = 0u; i < len; ++i) {
        while ((u->ISR & (1u << 5)) == 0u) { }
        data[i] = (uint8_t)u->RDR;
    }
    return true;
}

/** Start interrupt-driven transmit. */
bool usart_write_it_start(USART_TypeDef *usart, const uint8_t *data, size_t len, usart_cb_t cb, void *ctx) {
    if (!usart || !data || len == 0u) return false;
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return false;
    usart_it_state_t *st = &usart_it_state[idx];
    st->tx = data;
    st->rx = NULL;
    st->len = len;
    st->tx_idx = 0u;
    st->rx_idx = 0u;
    st->cb = cb;
    st->ctx = ctx;
    USART_TypeDef_real *u = usart_real(usart);
    u->CR1 |= (1u << 7); /* TXEIE */
    return true;
}

/** Start interrupt-driven receive. */
bool usart_read_it_start(USART_TypeDef *usart, uint8_t *data, size_t len, usart_cb_t cb, void *ctx) {
    if (!usart || !data || len == 0u) return false;
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return false;
    usart_it_state_t *st = &usart_it_state[idx];
    st->tx = NULL;
    st->rx = data;
    st->len = len;
    st->tx_idx = 0u;
    st->rx_idx = 0u;
    st->cb = cb;
    st->ctx = ctx;
    USART_TypeDef_real *u = usart_real(usart);
    u->CR1 |= (1u << 5); /* RXNEIE */
    return true;
}

/** DMA completion callback for USART transfers. */
static void usart_dma_cb(void *ctx, uint32_t flags) {
    (void)flags;
    usart_dma_state_t *st = (usart_dma_state_t *)ctx;
    if (st->pending > 0u) {
        st->pending--;
        if (st->pending == 0u) {
            usart_enable_dma(st->usart, false, false);
            if (st->cb) st->cb(st->ctx);
        }
    }
}

/** Start DMA-driven transmit. */
bool usart_write_dma_start(USART_TypeDef *usart, DMA_Channel_TypeDef *tx_ch, const uint8_t *data, size_t len, usart_cb_t cb, void *ctx) {
    if (!usart || !tx_ch || !data || len == 0u) return false;
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return false;
    usart_dma_state_t *st = &usart_dma_state[idx];
    st->usart = usart;
    st->cb = cb;
    st->ctx = ctx;
    st->pending = 1u;
    USART_TypeDef_real *u = usart_real(usart);
    rcc_ahb_enable(RCC_AHBENR_DMA1);
    dma_config_channel(tx_ch, DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE);
    dma_set_peripheral(tx_ch, (const void*)&u->TDR);
    dma_set_memory(tx_ch, (void *)data);
    dma_set_count(tx_ch, len);
    dma_set_callback(DMA1, dma_channel_index(tx_ch), usart_dma_cb, st);
    usart_enable_dma(usart, false, true);
    dma_enable(tx_ch, true);
    return true;
}

/** Start DMA-driven reception. */
bool usart_read_dma_start(USART_TypeDef *usart, DMA_Channel_TypeDef *rx_ch, uint8_t *data, size_t len, usart_cb_t cb, void *ctx) {
    if (!usart || !rx_ch || !data || len == 0u) return false;
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return false;
    usart_dma_state_t *st = &usart_dma_state[idx];
    st->usart = usart;
    st->cb = cb;
    st->ctx = ctx;
    st->pending = 1u;
    USART_TypeDef_real *u = usart_real(usart);
    rcc_ahb_enable(RCC_AHBENR_DMA1);
    dma_config_channel(rx_ch, DMA_CCR_MINC | DMA_CCR_TCIE);
    dma_set_peripheral(rx_ch, (const void*)&u->RDR);
    dma_set_memory(rx_ch, data);
    dma_set_count(rx_ch, len);
    dma_set_callback(DMA1, dma_channel_index(rx_ch), usart_dma_cb, st);
    usart_enable_dma(usart, true, false);
    dma_enable(rx_ch, true);
    return true;
}

/** Enable or disable DMA requests for RX/TX. */
void usart_enable_dma(USART_TypeDef *usart, bool rx, bool tx) {
    if (!usart) return;
    USART_TypeDef_real *u = usart_real(usart);
    uint32_t cr3 = u->CR3;
    if (rx) cr3 |= (1u << 6); else cr3 &= ~(1u << 6);
    if (tx) cr3 |= (1u << 7); else cr3 &= ~(1u << 7);
    u->CR3 = cr3;
}

/** Enable selected USART interrupts. */
void usart_enable_irq(USART_TypeDef *usart, uint32_t mask) {
    if (!usart) return;
    USART_TypeDef_real *u = usart_real(usart);
    uint32_t cr1 = u->CR1;
    if (mask & USART_IRQ_RXNE) cr1 |= (1u << 5); else cr1 &= ~(1u << 5);
    if (mask & USART_IRQ_TXE)  cr1 |= (1u << 7); else cr1 &= ~(1u << 7);
    if (mask & USART_IRQ_TC)   cr1 |= (1u << 6); else cr1 &= ~(1u << 6);
    if (mask & USART_IRQ_IDLE) cr1 |= (1u << 4); else cr1 &= ~(1u << 4);
    if (mask & USART_IRQ_PE)   cr1 |= (1u << 8); else cr1 &= ~(1u << 8);
    u->CR1 = cr1;
    uint32_t idx = usart_index(usart);
    if (idx < 4u) {
        IRQn_Type irqn = (idx < 2u) ? (IRQn_Type)(USART1_IRQn + idx) : USART3_4_IRQn;
        cm0_nvic_enable(irqn);
    }
}

/** Register generic interrupt callback. */
bool usart_attach_irq(USART_TypeDef *usart, usart_irq_cb_t cb, void *ctx) {
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return false;
    usart_irq_table[idx].cb = cb;
    usart_irq_table[idx].ctx = ctx;
    IRQn_Type irqn = (idx < 2u) ? (IRQn_Type)(USART1_IRQn + idx) : USART3_4_IRQn;
    cm0_nvic_enable(irqn);
    return true;
}

/** Remove generic interrupt callback. */
void usart_detach_irq(USART_TypeDef *usart) {
    uint32_t idx = usart_index(usart);
    if (idx >= 4u) return;
    usart_irq_table[idx].cb = NULL;
    usart_irq_table[idx].ctx = NULL;
}

/** Dispatch USART interrupts to handlers. */
static void usart_irq_dispatch(uint32_t idx) {
    USART_TypeDef_real *u = usart_real(idx == 0u ? USART1 : (idx == 1u ? USART2 : (idx == 2u ? USART3 : USART4)));
    uint32_t isr = u->ISR;
    usart_it_state_t *it = &usart_it_state[idx];
    if ((u->CR1 & (1u << 7)) && (isr & (1u << 7))) {
        if (it->tx && it->tx_idx < it->len) {
            u->TDR = it->tx[it->tx_idx++];
        }
        if (it->tx_idx >= it->len) {
            u->CR1 &= ~(1u << 7);
            if (it->cb) it->cb(it->ctx);
        }
    }
    if ((u->CR1 & (1u << 5)) && (isr & (1u << 5))) {
        uint8_t d = (uint8_t)u->RDR;
        if (it->rx && it->rx_idx < it->len) {
            it->rx[it->rx_idx++] = d;
        }
        if (it->rx_idx >= it->len) {
            u->CR1 &= ~(1u << 5);
            if (it->cb) it->cb(it->ctx);
        }
    }
    usart_irq_entry_t *e = &usart_irq_table[idx];
    if (e->cb) {
        e->cb(e->ctx, isr);
    }
}

/** USART1 global interrupt handler. */
void USART1_IRQHandler(void) { usart_irq_dispatch(0u); }
/** USART2 global interrupt handler. */
void USART2_IRQHandler(void) { usart_irq_dispatch(1u); }
/** Shared USART3/USART4 interrupt handler. */
void USART3_4_IRQHandler(void) {
    usart_irq_dispatch(2u);
    usart_irq_dispatch(3u);
}
