/*
 * SPI driver for STM32F0 without HAL.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "spi.h"
#include "rcc.h"
#include "gpio.h"
#include "cm0.h"


typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_TypeDef_real;

/** Convert generic SPI pointer to register structure. */
static inline SPI_TypeDef_real *spi_real(SPI_TypeDef *spi) {
    return (SPI_TypeDef_real *)spi;
}

typedef struct {
    spi_cb_t cb;
    void *ctx;
} spi_irq_entry_t;

static spi_irq_entry_t spi_irq_table[2];

typedef struct {
    const uint16_t *tx;
    uint16_t *rx;
    size_t len;
    size_t tx_idx;
    size_t rx_idx;
    spi_xfer_cb_t cb;
    void *ctx;
} spi_it_state_t;

typedef struct {
    SPI_TypeDef *spi;
    spi_xfer_cb_t cb;
    void *ctx;
    uint8_t pending;
} spi_dma_state_t;

static spi_it_state_t spi_it_state[2];
static spi_dma_state_t spi_dma_state[2];

/** Translate SPI instance to index (0 or 1). */
static uint32_t spi_index(SPI_TypeDef *spi) {
    if (spi == SPI1) return 0u;
    if (spi == SPI2) return 1u;
    return 2u;
}

/** Initialize an SPI peripheral with configuration. */
bool spi_init(SPI_TypeDef *spi, const spi_cfg_t *cfg) {
    if (!spi || !cfg) {
        return false;
    }
    if (cfg->datasize < 4u || cfg->datasize > 16u) {
        return false;
    }
    SPI_TypeDef_real *s = spi_real(spi);
    if (spi == SPI1) {
        rcc_apb2_enable(RCC_APB2ENR_SPI1);
    } else if (spi == SPI2) {
        rcc_apb1_enable(RCC_APB1ENR_SPI2);
    } else {
        return false;
    }

    s->CR1 = 0u;
    s->CR2 = 0u;
    uint32_t cr1 = 0u;
    uint32_t cr2 = 0u;
    if (cfg->mode == SPI_MODE_MASTER) {
        cr1 |= (1u << 2); /* MSTR */
    }
    if (cfg->cpol == SPI_CPOL_HIGH) {
        cr1 |= (1u << 1);
    }
    if (cfg->cpha == SPI_CPHA_2EDGE) {
        cr1 |= (1u << 0);
    }
    if (cfg->first_bit == SPI_FIRST_LSB) {
        cr1 |= (1u << 7);
    }
    if (cfg->nss == SPI_NSS_SOFT) {
        cr1 |= (1u << 9) | (1u << 8); /* SSM | SSI */
    }
    switch (cfg->direction) {
    case SPI_DIR_HALF_DUPLEX_RX:
        cr1 |= (1u << 15); /* BIDIMODE */
        break;
    case SPI_DIR_HALF_DUPLEX_TX:
        cr1 |= (1u << 15) | (1u << 14); /* BIDIMODE | BIDIOE */
        break;
    case SPI_DIR_SIMPLEX_RX:
        cr1 |= (1u << 10); /* RXONLY */
        break;
    case SPI_DIR_SIMPLEX_TX:
        /* nothing special; just ignore RX */
        break;
    default:
        break;
    }
    if (cfg->crc_enable) {
        cr1 |= (1u << 13); /* CRCEN */
        s->CRCPR = cfg->crc_poly;
    }
    cr2 |= ((uint32_t)(cfg->datasize - 1u) & 0xFu) << 8; /* DS */
    if (cfg->nss == SPI_NSS_HARD && cfg->mode == SPI_MODE_MASTER) {
        cr2 |= (1u << 2); /* SSOE */
    }
    s->CR1 = cr1;
    s->CR2 = cr2;
    return true;
}

/** Enable or disable the SPI peripheral. */
void spi_enable(SPI_TypeDef *spi, bool enable) {
    SPI_TypeDef_real *s = spi_real(spi);
    if (enable) {
        s->CR1 |= (1u << 6); /* SPE */
    } else {
        s->CR1 &= ~(1u << 6);
    }
}

/** Perform a blocking SPI transfer. */
uint16_t spi_transfer(SPI_TypeDef *spi, uint16_t data) {
    SPI_TypeDef_real *s = spi_real(spi);
    while ((s->SR & (1u << 1)) == 0u) {
    }
    s->DR = data;
    while ((s->SR & (1u << 0)) == 0u) {
    }
    return (uint16_t)s->DR;
}

/** Enable specific SPI interrupts. */
void spi_enable_irq(SPI_TypeDef *spi, uint32_t mask) {
    SPI_TypeDef_real *s = spi_real(spi);
    uint32_t cr2 = s->CR2;
    if (mask & SPI_IRQ_RXNE) {
        cr2 |= (1u << 6);
    } else {
        cr2 &= ~(1u << 6);
    }
    if (mask & SPI_IRQ_TXE) {
        cr2 |= (1u << 7);
    } else {
        cr2 &= ~(1u << 7);
    }
    if (mask & SPI_IRQ_ERR) {
        cr2 |= (1u << 5);
    } else {
        cr2 &= ~(1u << 5);
    }
    s->CR2 = cr2;
    uint32_t idx = spi_index(spi);
    if (idx < 2u) {
        cm0_nvic_enable((IRQn_Type)(SPI1_IRQn + idx));
    }
}

/** Register a callback for SPI interrupts. */
bool spi_attach_irq(SPI_TypeDef *spi, spi_cb_t cb, void *ctx) {
    uint32_t idx = spi_index(spi);
    if (idx >= 2u) {
        return false;
    }
    spi_irq_table[idx].cb = cb;
    spi_irq_table[idx].ctx = ctx;
    return true;
}

/** Remove SPI interrupt callback. */
void spi_detach_irq(SPI_TypeDef *spi) {
    uint32_t idx = spi_index(spi);
    if (idx < 2u) {
        spi_irq_table[idx].cb = NULL;
        spi_irq_table[idx].ctx = NULL;
        cm0_nvic_disable((IRQn_Type)(SPI1_IRQn + idx));
    }
}

/** Enable or disable DMA requests. */
void spi_enable_dma(SPI_TypeDef *spi, bool rx, bool tx) {
    SPI_TypeDef_real *s = spi_real(spi);
    uint32_t cr2 = s->CR2;
    if (rx) {
        cr2 |= (1u << 0);
    } else {
        cr2 &= ~(1u << 0);
    }
    if (tx) {
        cr2 |= (1u << 1);
    } else {
        cr2 &= ~(1u << 1);
    }
    s->CR2 = cr2;
}

/** Retrieve SPI error flags. */
uint32_t spi_get_error(SPI_TypeDef *spi) {
    SPI_TypeDef_real *s = spi_real(spi);
    uint32_t sr = s->SR;
    uint32_t err = 0u;
    if (sr & (1u << 6)) err |= SPI_ERROR_OVR;
    if (sr & (1u << 5)) err |= SPI_ERROR_MODF;
    if (sr & (1u << 3)) err |= SPI_ERROR_UDR;
    if (sr & (1u << 4)) err |= SPI_ERROR_CRC;
    return err;
}

/** Clear specified SPI error flags. */
void spi_clear_error(SPI_TypeDef *spi, uint32_t errors) {
    SPI_TypeDef_real *s = spi_real(spi);
    if (errors & SPI_ERROR_OVR) {
        (void)s->DR;
        (void)s->SR;
    }
    if (errors & SPI_ERROR_MODF) {
        (void)s->SR;
        s->CR1 |= 0u; /* write to clear */
    }
    if (errors & SPI_ERROR_UDR) {
        (void)s->SR;
    }
    if (errors & SPI_ERROR_CRC) {
        s->SR &= ~(1u << 4);
    }
}

/** Start interrupt-driven SPI transfer. */
bool spi_transfer_it_start(SPI_TypeDef *spi, const uint16_t *tx, uint16_t *rx, size_t len, spi_xfer_cb_t cb, void *ctx) {
    if (!spi || len == 0u) return false;
    uint32_t idx_spi = spi_index(spi);
    if (idx_spi >= 2u) return false;
    spi_it_state_t *st = &spi_it_state[idx_spi];
    st->tx = tx;
    st->rx = rx;
    st->len = len;
    st->tx_idx = 0u;
    st->rx_idx = 0u;
    st->cb = cb;
    st->ctx = ctx;
    SPI_TypeDef_real *s = spi_real(spi);
    s->CR2 |= (1u << 6) | (1u << 7); /* RXNEIE | TXEIE */
    if (tx) {
        s->DR = tx[0];
    } else {
        s->DR = 0xFFFFu;
    }
    st->tx_idx = 1u;
    return true;
}

/** Obtain DMA channel index for the given channel. */
static uint8_t dma_channel_index(DMA_Channel_TypeDef *ch) {
    return (uint8_t)(ch - &DMA1->CH[0]) + 1u;
}

/** DMA completion callback for SPI transfers. */
static void spi_dma_cb(void *ctx, uint32_t flags) {
    (void)flags;
    spi_dma_state_t *st = (spi_dma_state_t *)ctx;
    if (st->pending > 0u) {
        st->pending--;
        if (st->pending == 0u) {
            spi_enable_dma(st->spi, false, false);
            if (st->cb) st->cb(st->ctx);
        }
    }
}

/** Start DMA-driven SPI transfer. */
bool spi_transfer_dma_start(SPI_TypeDef *spi, DMA_Channel_TypeDef *tx_ch, DMA_Channel_TypeDef *rx_ch, const uint16_t *tx, uint16_t *rx, size_t len, spi_xfer_cb_t cb, void *ctx) {
    if (!spi || len == 0u) return false;
    uint32_t idx_spi = spi_index(spi);
    if (idx_spi >= 2u) return false;
    spi_dma_state_t *st = &spi_dma_state[idx_spi];
    st->spi = spi;
    st->cb = cb;
    st->ctx = ctx;
    st->pending = 0u;
    SPI_TypeDef_real *s = spi_real(spi);
    if (rx_ch && rx) {
        rcc_ahb_enable(RCC_AHBENR_DMA1);
        dma_config_channel(rx_ch, DMA_CCR_MINC | DMA_CCR_TCIE);
        dma_set_peripheral(rx_ch, (const void*)&s->DR);
        dma_set_memory(rx_ch, rx);
        dma_set_count(rx_ch, len);
        dma_set_callback(DMA1, dma_channel_index(rx_ch), spi_dma_cb, st);
        st->pending++;
    }
    if (tx_ch && tx) {
        rcc_ahb_enable(RCC_AHBENR_DMA1);
        dma_config_channel(tx_ch, DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE);
        dma_set_peripheral(tx_ch, (const void*)&s->DR);
        dma_set_memory(tx_ch, (void *)tx);
        dma_set_count(tx_ch, len);
        dma_set_callback(DMA1, dma_channel_index(tx_ch), spi_dma_cb, st);
        st->pending++;
    }
    if (st->pending == 0u) return false;
    spi_enable_dma(spi, rx_ch != NULL && rx != NULL, tx_ch != NULL && tx != NULL);
    if (rx_ch && rx) dma_enable(rx_ch, true);
    if (tx_ch && tx) dma_enable(tx_ch, true);
    return true;
}

/** Initialize a GPIO as chip-select output. */
void spi_cs_init(GPIO_TypeDef *port, uint8_t pin) {
    gpio_config(port, pin, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_HIGH,
    });
    gpio_write(port, pin, 1u);
}

/** Control the chip-select line. */
void spi_cs_select(GPIO_TypeDef *port, uint8_t pin, bool select) {
    gpio_write(port, pin, select ? 0u : 1u);
}

/** Dispatch SPI interrupts to registered callbacks. */
static void spi_irq_dispatch(uint32_t idx) {
    SPI_TypeDef_real *s = spi_real(idx == 0u ? SPI1 : SPI2);
    uint32_t sr = s->SR;
    spi_it_state_t *it = &spi_it_state[idx];
    if ((s->CR2 & ((1u<<6)|(1u<<7))) != 0u) {
        if ((sr & (1u<<1)) && it->tx_idx < it->len) {
            uint16_t d = it->tx ? it->tx[it->tx_idx] : 0xFFFFu;
            s->DR = d;
            it->tx_idx++;
        }
        if (sr & (1u<<0)) {
            uint16_t d = (uint16_t)s->DR;
            if (it->rx && it->rx_idx < it->len) {
                it->rx[it->rx_idx] = d;
            }
            it->rx_idx++;
            if (it->rx_idx >= it->len) {
                s->CR2 &= ~((1u<<6)|(1u<<7));
                if (it->cb) it->cb(it->ctx);
            }
        }
    }
    if (sr & (1u << 4)) {
        s->SR &= ~(1u << 4); /* clear CRCERR */
    }
    if (sr & (1u << 6)) {
        (void)s->DR;
        (void)s->SR;
    }
    spi_irq_entry_t *e = &spi_irq_table[idx];
    if (e->cb) {
        e->cb(e->ctx, sr);
    }
}

/** SPI1 global interrupt handler. */
void SPI1_IRQHandler(void) { spi_irq_dispatch(0u); }
/** SPI2 global interrupt handler. */
void SPI2_IRQHandler(void) { spi_irq_dispatch(1u); }

/** Example: read JEDEC ID from SPI device. */
void spi_example_jedec_id(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    spi_init(SPI1, &cfg);
    spi_cs_init(GPIOC, 0u);
    spi_enable(SPI1, true);
    spi_cs_select(GPIOC, 0u, true);
    uint8_t cmd = 0x9Fu;
    uint8_t id[3];
    id[0] = (uint8_t)spi_transfer(SPI1, cmd);
    id[1] = (uint8_t)spi_transfer(SPI1, 0xFFu);
    id[2] = (uint8_t)spi_transfer(SPI1, 0xFFu);
    spi_cs_select(GPIOC, 0u, false);
    (void)id;
}

/** Example: send a stream of bytes over SPI. */
void spi_example_display_stream(const uint8_t *data, size_t len) {
    if (!data) {
        return;
    }
    for (size_t i = 0u; i < len; ++i) {
        (void)spi_transfer(SPI1, data[i]);
    }
}
