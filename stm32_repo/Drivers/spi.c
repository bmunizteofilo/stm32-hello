/*
 * SPI driver for STM32F0 without HAL.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "spi.h"
#include "gpio.h"
#include "cm0.h"

#if defined(STM32F0_FIRMWARE)

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

#define RCC_BASE  0x40021000u
#define RCC ((RCC_TypeDef *)RCC_BASE)

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
} RCC_TypeDef;

static inline SPI_TypeDef_real *spi_real(SPI_TypeDef *spi) {
    return (SPI_TypeDef_real *)spi;
}

typedef struct {
    spi_cb_t cb;
    void *ctx;
} spi_irq_entry_t;

static spi_irq_entry_t spi_irq_table[2];

static uint32_t spi_index(SPI_TypeDef *spi) {
    if (spi == SPI1) return 0u;
    if (spi == SPI2) return 1u;
    return 2u;
}

bool spi_init(SPI_TypeDef *spi, const spi_cfg_t *cfg) {
    if (!spi || !cfg) {
        return false;
    }
    if (cfg->datasize < 4u || cfg->datasize > 16u) {
        return false;
    }
    SPI_TypeDef_real *s = spi_real(spi);
    if (spi == SPI1) {
        RCC->APB2ENR |= (1u << 12); /* SPI1EN */
    } else if (spi == SPI2) {
        RCC->APB1ENR |= (1u << 14); /* SPI2EN */
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

void spi_enable(SPI_TypeDef *spi, bool enable) {
    SPI_TypeDef_real *s = spi_real(spi);
    if (enable) {
        s->CR1 |= (1u << 6); /* SPE */
    } else {
        s->CR1 &= ~(1u << 6);
    }
}

uint16_t spi_transfer(SPI_TypeDef *spi, uint16_t data) {
    SPI_TypeDef_real *s = spi_real(spi);
    while ((s->SR & (1u << 1)) == 0u) {
    }
    s->DR = data;
    while ((s->SR & (1u << 0)) == 0u) {
    }
    return (uint16_t)s->DR;
}

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

bool spi_attach_irq(SPI_TypeDef *spi, spi_cb_t cb, void *ctx) {
    uint32_t idx = spi_index(spi);
    if (idx >= 2u) {
        return false;
    }
    spi_irq_table[idx].cb = cb;
    spi_irq_table[idx].ctx = ctx;
    return true;
}

void spi_detach_irq(SPI_TypeDef *spi) {
    uint32_t idx = spi_index(spi);
    if (idx < 2u) {
        spi_irq_table[idx].cb = NULL;
        spi_irq_table[idx].ctx = NULL;
        cm0_nvic_disable((IRQn_Type)(SPI1_IRQn + idx));
    }
}

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

void spi_cs_init(GPIO_TypeDef *port, uint8_t pin) {
    gpio_config(port, pin, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_HIGH,
    });
    gpio_write(port, pin, 1u);
}

void spi_cs_select(GPIO_TypeDef *port, uint8_t pin, bool select) {
    gpio_write(port, pin, select ? 0u : 1u);
}

static void spi_irq_dispatch(uint32_t idx) {
    SPI_TypeDef_real *s = spi_real(idx == 0u ? SPI1 : SPI2);
    uint32_t sr = s->SR;
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

void SPI1_IRQHandler(void) { spi_irq_dispatch(0u); }
void SPI2_IRQHandler(void) { spi_irq_dispatch(1u); }

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

void spi_example_display_stream(const uint8_t *data, size_t len) {
    if (!data) {
        return;
    }
    for (size_t i = 0u; i < len; ++i) {
        (void)spi_transfer(SPI1, data[i]);
    }
}

#else /* STM32F0_FIRMWARE */

bool spi_init(SPI_TypeDef *spi, const spi_cfg_t *cfg) {
    if (!spi || !cfg) return false;
    if (cfg->datasize < 4u || cfg->datasize > 16u) return false;
    return true;
}
void spi_enable(SPI_TypeDef *spi, bool enable) { (void)spi; (void)enable; }
uint16_t spi_transfer(SPI_TypeDef *spi, uint16_t data) { (void)spi; return data; }
void spi_enable_irq(SPI_TypeDef *spi, uint32_t mask) { (void)spi; (void)mask; }
bool spi_attach_irq(SPI_TypeDef *spi, spi_cb_t cb, void *ctx) {
    (void)spi; (void)cb; (void)ctx; return false;
}
void spi_detach_irq(SPI_TypeDef *spi) { (void)spi; }
void spi_enable_dma(SPI_TypeDef *spi, bool rx, bool tx) {
    (void)spi; (void)rx; (void)tx;
}
uint32_t spi_get_error(SPI_TypeDef *spi) { (void)spi; return 0u; }
void spi_clear_error(SPI_TypeDef *spi, uint32_t errors) {
    (void)spi; (void)errors;
}
void spi_cs_init(GPIO_TypeDef *port, uint8_t pin) {
    (void)port; (void)pin;
}
void spi_cs_select(GPIO_TypeDef *port, uint8_t pin, bool select) {
    (void)port; (void)pin; (void)select;
}
void spi_example_jedec_id(void) {}
void spi_example_display_stream(const uint8_t *data, size_t len) {
    (void)data; (void)len;
}

#endif /* STM32F0_FIRMWARE */
