#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dma.h"

/* Forward declarations for host builds */
typedef struct SPI_TypeDef SPI_TypeDef;
typedef struct GPIO_TypeDef GPIO_TypeDef;

#define SPI1 ((SPI_TypeDef *)0x40013000u)
#define SPI2 ((SPI_TypeDef *)0x40003800u)

enum spi_mode_t {
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE,
};

enum spi_dir_t {
    SPI_DIR_FULL_DUPLEX,
    SPI_DIR_HALF_DUPLEX_RX,
    SPI_DIR_HALF_DUPLEX_TX,
    SPI_DIR_SIMPLEX_RX,
    SPI_DIR_SIMPLEX_TX,
};

enum spi_cpol_t {
    SPI_CPOL_LOW,
    SPI_CPOL_HIGH,
};

enum spi_cpha_t {
    SPI_CPHA_1EDGE,
    SPI_CPHA_2EDGE,
};

enum spi_first_bit_t {
    SPI_FIRST_MSB,
    SPI_FIRST_LSB,
};

enum spi_nss_t {
    SPI_NSS_HARD,
    SPI_NSS_SOFT,
};

#define SPI_IRQ_RXNE 0x1u
#define SPI_IRQ_TXE  0x2u
#define SPI_IRQ_ERR  0x4u

#define SPI_ERROR_OVR   0x1u
#define SPI_ERROR_MODF  0x2u
#define SPI_ERROR_UDR   0x4u
#define SPI_ERROR_CRC   0x8u


typedef struct {
    enum spi_mode_t mode;
    enum spi_dir_t direction;
    enum spi_cpol_t cpol;
    enum spi_cpha_t cpha;
    uint8_t datasize; /* 4..16 */
    enum spi_first_bit_t first_bit;
    enum spi_nss_t nss;
    bool crc_enable;
    uint16_t crc_poly;
} spi_cfg_t;

/** Callback type for SPI interrupts. */
typedef void (*spi_cb_t)(void *ctx, uint32_t sr);

#endif /* SPI_H */
/** Initialize an SPI peripheral. */
bool spi_init(SPI_TypeDef *spi, const spi_cfg_t *cfg);
/** Enable or disable SPI. */
void spi_enable(SPI_TypeDef *spi, bool enable);
/** Transfer a word over SPI in polling mode. */
uint16_t spi_transfer(SPI_TypeDef *spi, uint16_t data);
/** Enable specific SPI interrupts. */
void spi_enable_irq(SPI_TypeDef *spi, uint32_t mask);
/** Register an SPI interrupt callback. */
bool spi_attach_irq(SPI_TypeDef *spi, spi_cb_t cb, void *ctx);
/** Detach SPI interrupt callback. */
void spi_detach_irq(SPI_TypeDef *spi);
/** Enable or disable DMA for SPI. */
void spi_enable_dma(SPI_TypeDef *spi, bool rx, bool tx);
/** Retrieve error flags from SPI. */
uint32_t spi_get_error(SPI_TypeDef *spi);
/** Clear specific SPI error flags. */
void spi_clear_error(SPI_TypeDef *spi, uint32_t errors);
/** Initialize a GPIO pin as chip-select. */
void spi_cs_init(GPIO_TypeDef *port, uint8_t pin);
/** Control the chip-select pin. */
void spi_cs_select(GPIO_TypeDef *port, uint8_t pin, bool select);

/** Callback type for asynchronous SPI transfers. */
typedef void (*spi_xfer_cb_t)(void *ctx);
/** Start interrupt-driven SPI transfer. */
bool spi_transfer_it_start(SPI_TypeDef *spi, const uint16_t *tx, uint16_t *rx, size_t len, spi_xfer_cb_t cb, void *ctx);
/** Start DMA-driven SPI transfer. */
bool spi_transfer_dma_start(SPI_TypeDef *spi, DMA_Channel_TypeDef *tx_ch, DMA_Channel_TypeDef *rx_ch, const uint16_t *tx, uint16_t *rx, size_t len, spi_xfer_cb_t cb, void *ctx);

/** Example: read JEDEC ID from SPI flash. */
void spi_example_jedec_id(void);
/** Example: stream data to SPI peripheral. */
void spi_example_display_stream(const uint8_t *data, size_t len);
