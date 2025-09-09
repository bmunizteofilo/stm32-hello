/*
 * I2C driver for STM32F0 without HAL.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "i2c.h"
#include "rcc.h"
#include "cm0.h"

#define I2C_IT_BUF_SIZE 64u

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
} I2C_TypeDef_real;

static inline I2C_TypeDef_real *i2c_real(I2C_TypeDef *i2c) {
    return (I2C_TypeDef_real *)i2c;
}

typedef struct {
    uint8_t buf[I2C_IT_BUF_SIZE];
    size_t head;
    size_t tail;
} i2c_ring_t;

static void ring_reset(i2c_ring_t *r) { r->head = r->tail = 0u; }
static bool ring_empty(const i2c_ring_t *r) { return r->head == r->tail; }
static bool ring_full(const i2c_ring_t *r) { return ((r->head + 1u) % I2C_IT_BUF_SIZE) == r->tail; }
static bool ring_push(i2c_ring_t *r, uint8_t v) {
    if (ring_full(r)) return false;
    r->buf[r->head] = v;
    r->head = (r->head + 1u) % I2C_IT_BUF_SIZE;
    return true;
}
static bool ring_pop(i2c_ring_t *r, uint8_t *v) {
    if (ring_empty(r)) return false;
    *v = r->buf[r->tail];
    r->tail = (r->tail + 1u) % I2C_IT_BUF_SIZE;
    return true;
}
static size_t ring_count(const i2c_ring_t *r) {
    return (r->head + I2C_IT_BUF_SIZE - r->tail) % I2C_IT_BUF_SIZE;
}

typedef struct {
    i2c_ring_t tx;
    i2c_ring_t rx;
    size_t expected;
    i2c_cb_t cb;
    void *ctx;
} i2c_it_state_t;

typedef struct {
    I2C_TypeDef *i2c;
    i2c_cb_t cb;
    void *ctx;
    uint8_t pending;
} i2c_dma_state_t;

static i2c_it_state_t i2c_it_state[2];
static i2c_dma_state_t i2c_dma_state[2];

static uint32_t i2c_index(I2C_TypeDef *i2c) {
    if (i2c == I2C1) return 0u;
    if (i2c == I2C2) return 1u;
    return 2u;
}

static void i2c_start(I2C_TypeDef_real *s, uint8_t addr, size_t nbytes, bool read) {
    s->CR2 = ((uint32_t)addr << 1) | (nbytes << 16) | (read ? (1u<<10) : 0u) | (1u<<13);
}

bool i2c_init(I2C_TypeDef *i2c, const i2c_cfg_t *cfg) {
    if (!i2c || !cfg) return false;
    I2C_TypeDef_real *s = i2c_real(i2c);
    if (i2c == I2C1) {
        rcc_apb1_enable(RCC_APB1ENR_I2C1);
    } else if (i2c == I2C2) {
        rcc_apb1_enable(RCC_APB1ENR_I2C2);
    } else {
        return false;
    }
    s->CR1 = 0u;
    s->TIMINGR = cfg->timing;
    if (cfg->addressing_mode == I2C_ADDR_10BIT) {
        s->OAR1 = (1u<<15) | cfg->own_address;
    } else {
        s->OAR1 = (1u<<14) | ((uint32_t)cfg->own_address << 1);
    }
    return true;
}

void i2c_enable(I2C_TypeDef *i2c, bool en) {
    I2C_TypeDef_real *s = i2c_real(i2c);
    if (en) s->CR1 |= 1u; else s->CR1 &= ~1u;
}

bool i2c_write_poll(I2C_TypeDef *i2c, uint8_t addr, const uint8_t *data, size_t len) {
    if (!i2c || !data || len == 0u) return false;
    I2C_TypeDef_real *s = i2c_real(i2c);
    i2c_start(s, addr, len, false);
    for (size_t i = 0u; i < len; ++i) {
        while ((s->ISR & (1u<<1)) == 0u) {}
        s->TXDR = data[i];
    }
    while ((s->ISR & (1u<<6)) == 0u) {}
    s->CR2 |= (1u<<15);
    return true;
}

bool i2c_read_poll(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, size_t len) {
    if (!i2c || !data || len == 0u) return false;
    I2C_TypeDef_real *s = i2c_real(i2c);
    i2c_start(s, addr, len, true);
    for (size_t i = 0u; i < len; ++i) {
        while ((s->ISR & (1u<<2)) == 0u) {}
        data[i] = (uint8_t)s->RXDR;
    }
    s->CR2 |= (1u<<15);
    return true;
}

bool i2c_write_it_start(I2C_TypeDef *i2c, uint8_t addr, const uint8_t *data, size_t len, i2c_cb_t cb, void *ctx) {
    if (!i2c || !data || len == 0u || len >= I2C_IT_BUF_SIZE) return false;
    uint32_t idx = i2c_index(i2c);
    if (idx >= 2u) return false;
    i2c_it_state_t *st = &i2c_it_state[idx];
    ring_reset(&st->tx);
    for (size_t i = 0u; i < len; ++i) ring_push(&st->tx, data[i]);
    st->expected = len;
    st->cb = cb;
    st->ctx = ctx;
    I2C_TypeDef_real *s = i2c_real(i2c);
    i2c_start(s, addr, len, false);
    s->CR1 |= (1u<<1); /* TXIE */
    cm0_nvic_enable((IRQn_Type)(I2C1_IRQn + idx));
    return true;
}

bool i2c_read_it_start(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, size_t len, i2c_cb_t cb, void *ctx) {
    (void)ctx;
    if (!i2c || !data || len == 0u || len >= I2C_IT_BUF_SIZE) return false;
    uint32_t idx = i2c_index(i2c);
    if (idx >= 2u) return false;
    i2c_it_state_t *st = &i2c_it_state[idx];
    ring_reset(&st->rx);
    st->expected = len;
    st->cb = cb;
    st->ctx = data; /* use ctx to pass buffer */
    I2C_TypeDef_real *s = i2c_real(i2c);
    i2c_start(s, addr, len, true);
    s->CR1 |= (1u<<2); /* RXIE */
    cm0_nvic_enable((IRQn_Type)(I2C1_IRQn + idx));
    return true;
}

static uint8_t dma_channel_index(DMA_Channel_TypeDef *ch) {
    return (uint8_t)(ch - &DMA1->CH[0]) + 1u;
}

static void i2c_dma_cb(void *ctx, uint32_t flags) {
    (void)flags;
    i2c_dma_state_t *st = (i2c_dma_state_t *)ctx;
    if (st->pending > 0u) {
        st->pending--;
        if (st->pending == 0u) {
            if (st->cb) st->cb(st->ctx);
        }
    }
}

bool i2c_write_dma_start(I2C_TypeDef *i2c, DMA_Channel_TypeDef *tx_ch, uint8_t addr, const uint8_t *data, size_t len, i2c_cb_t cb, void *ctx) {
    if (!i2c || !tx_ch || !data || len == 0u) return false;
    uint32_t idx = i2c_index(i2c);
    if (idx >= 2u) return false;
    i2c_dma_state_t *st = &i2c_dma_state[idx];
    st->i2c = i2c;
    st->cb = cb;
    st->ctx = ctx;
    st->pending = 1u;
    I2C_TypeDef_real *s = i2c_real(i2c);
    rcc_ahb_enable(RCC_AHBENR_DMA1);
    dma_config_channel(tx_ch, DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE);
    dma_set_peripheral(tx_ch, (const void*)&s->TXDR);
    dma_set_memory(tx_ch, (void *)data);
    dma_set_count(tx_ch, len);
    dma_set_callback(DMA1, dma_channel_index(tx_ch), i2c_dma_cb, st);
    i2c_start(s, addr, len, false);
    dma_enable(tx_ch, true);
    return true;
}

bool i2c_read_dma_start(I2C_TypeDef *i2c, DMA_Channel_TypeDef *rx_ch, uint8_t addr, uint8_t *data, size_t len, i2c_cb_t cb, void *ctx) {
    if (!i2c || !rx_ch || !data || len == 0u) return false;
    uint32_t idx = i2c_index(i2c);
    if (idx >= 2u) return false;
    i2c_dma_state_t *st = &i2c_dma_state[idx];
    st->i2c = i2c;
    st->cb = cb;
    st->ctx = ctx;
    st->pending = 1u;
    I2C_TypeDef_real *s = i2c_real(i2c);
    rcc_ahb_enable(RCC_AHBENR_DMA1);
    dma_config_channel(rx_ch, DMA_CCR_MINC | DMA_CCR_TCIE);
    dma_set_peripheral(rx_ch, (const void*)&s->RXDR);
    dma_set_memory(rx_ch, data);
    dma_set_count(rx_ch, len);
    dma_set_callback(DMA1, dma_channel_index(rx_ch), i2c_dma_cb, st);
    i2c_start(s, addr, len, true);
    dma_enable(rx_ch, true);
    return true;
}

static void i2c_irq_dispatch(uint32_t idx) {
    I2C_TypeDef_real *s = i2c_real(idx == 0u ? I2C1 : I2C2);
    i2c_it_state_t *st = &i2c_it_state[idx];
    uint32_t isr = s->ISR;
    if ((s->CR1 & (1u<<1)) && (isr & (1u<<1))) {
        uint8_t v;
        if (ring_pop(&st->tx, &v)) {
            s->TXDR = v;
        }
        if (ring_empty(&st->tx)) {
            s->CR1 &= ~(1u<<1);
            s->CR2 |= (1u<<15);
            if (st->cb) st->cb(st->ctx);
        }
    }
    if ((s->CR1 & (1u<<2)) && (isr & (1u<<2))) {
        uint8_t v = (uint8_t)s->RXDR;
        ring_push(&st->rx, v);
        if (ring_count(&st->rx) >= st->expected) {
            uint8_t *buf = (uint8_t *)st->ctx;
            for (size_t i = 0u; i < st->expected; ++i) {
                ring_pop(&st->rx, &buf[i]);
            }
            s->CR1 &= ~(1u<<2);
            s->CR2 |= (1u<<15);
            if (st->cb) st->cb(NULL);
        }
    }
}

void I2C1_IRQHandler(void) { i2c_irq_dispatch(0u); }
void I2C2_IRQHandler(void) { i2c_irq_dispatch(1u); }

void i2c_example_write_poll(void) {
    uint8_t data[2] = {0xAA, 0x55};
    (void)i2c_write_poll(I2C1, 0x50u, data, 2u);
}

void i2c_example_read_poll(void) {
    uint8_t data[2];
    (void)i2c_read_poll(I2C1, 0x50u, data, 2u);
}

static void it_cb(void *ctx) { (void)ctx; }

void i2c_example_write_it(void) {
    static uint8_t data[3] = {1,2,3};
    i2c_write_it_start(I2C1, 0x50u, data, 3u, it_cb, NULL);
}

void i2c_example_read_it(void) {
    static uint8_t data[3];
    i2c_read_it_start(I2C1, 0x50u, data, 3u, it_cb, data);
}

void i2c_example_write_dma(void) {
    static uint8_t data[4] = {1,2,3,4};
    i2c_write_dma_start(I2C1, &DMA1->CH[2], 0x50u, data, 4u, it_cb, NULL);
}

void i2c_example_read_dma(void) {
    static uint8_t data[4];
    i2c_read_dma_start(I2C1, &DMA1->CH[3], 0x50u, data, 4u, it_cb, NULL);
}

