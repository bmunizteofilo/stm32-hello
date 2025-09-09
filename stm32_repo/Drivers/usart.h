#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dma.h"

/* Forward declaration for host builds */
typedef struct USART_TypeDef USART_TypeDef;

#define USART1 ((USART_TypeDef *)0x40013800u)
#define USART2 ((USART_TypeDef *)0x40004400u)
#define USART3 ((USART_TypeDef *)0x40004800u)
#define USART4 ((USART_TypeDef *)0x40004C00u)

enum usart_word_length {
    USART_WORD_8,
    USART_WORD_9,
};

enum usart_stop_bits {
    USART_STOP_1,
    USART_STOP_0_5,
    USART_STOP_2,
    USART_STOP_1_5,
};

enum usart_parity {
    USART_PARITY_NONE,
    USART_PARITY_EVEN,
    USART_PARITY_ODD,
};

enum usart_mode {
    USART_MODE_RX  = 1u,
    USART_MODE_TX  = 2u,
    USART_MODE_TX_RX = 3u,
};

enum usart_hw_flow {
    USART_HW_NONE,
    USART_HW_RTS,
    USART_HW_CTS,
    USART_HW_RTS_CTS,
};

enum usart_oversampling {
    USART_OVER16,
    USART_OVER8,
};

typedef struct {
    uint32_t baudrate;
    enum usart_word_length word_length;
    enum usart_stop_bits stop_bits;
    enum usart_parity parity;
    enum usart_mode mode;
    enum usart_hw_flow hw_flow;
    enum usart_oversampling oversampling;
} usart_cfg_t;

typedef void (*usart_cb_t)(void *ctx);
typedef void (*usart_irq_cb_t)(void *ctx, uint32_t isr);

#define USART_IRQ_RXNE 0x1u
#define USART_IRQ_TXE  0x2u
#define USART_IRQ_TC   0x4u
#define USART_IRQ_IDLE 0x8u
#define USART_IRQ_PE   0x10u

bool usart_init(USART_TypeDef *usart, const usart_cfg_t *cfg);
void usart_enable(USART_TypeDef *usart, bool en);

bool usart_write_poll(USART_TypeDef *usart, const uint8_t *data, size_t len);
bool usart_read_poll(USART_TypeDef *usart, uint8_t *data, size_t len);

bool usart_write_it_start(USART_TypeDef *usart, const uint8_t *data, size_t len, usart_cb_t cb, void *ctx);
bool usart_read_it_start(USART_TypeDef *usart, uint8_t *data, size_t len, usart_cb_t cb, void *ctx);

bool usart_write_dma_start(USART_TypeDef *usart, DMA_Channel_TypeDef *tx_ch, const uint8_t *data, size_t len, usart_cb_t cb, void *ctx);
bool usart_read_dma_start(USART_TypeDef *usart, DMA_Channel_TypeDef *rx_ch, uint8_t *data, size_t len, usart_cb_t cb, void *ctx);

void usart_enable_dma(USART_TypeDef *usart, bool rx, bool tx);
void usart_enable_irq(USART_TypeDef *usart, uint32_t mask);
bool usart_attach_irq(USART_TypeDef *usart, usart_irq_cb_t cb, void *ctx);
void usart_detach_irq(USART_TypeDef *usart);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_4_IRQHandler(void);

#endif /* USART_H */
