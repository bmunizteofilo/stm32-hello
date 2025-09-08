#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declarations for host builds */
typedef struct SPI_TypeDef SPI_TypeDef;
typedef struct GPIO_TypeDef GPIO_TypeDef;

#if defined(STM32F0_FIRMWARE)
#define SPI1 ((SPI_TypeDef *)0x40013000u)
#define SPI2 ((SPI_TypeDef *)0x40003800u)
#endif

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

typedef void (*spi_cb_t)(void *ctx, uint32_t sr);

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

bool spi_init(SPI_TypeDef *spi, const spi_cfg_t *cfg);
void spi_enable(SPI_TypeDef *spi, bool enable);
uint16_t spi_transfer(SPI_TypeDef *spi, uint16_t data);
void spi_enable_irq(SPI_TypeDef *spi, uint32_t mask);
bool spi_attach_irq(SPI_TypeDef *spi, spi_cb_t cb, void *ctx);
void spi_detach_irq(SPI_TypeDef *spi);
void spi_enable_dma(SPI_TypeDef *spi, bool rx, bool tx);
uint32_t spi_get_error(SPI_TypeDef *spi);
void spi_clear_error(SPI_TypeDef *spi, uint32_t errors);
void spi_cs_init(GPIO_TypeDef *port, uint8_t pin);
void spi_cs_select(GPIO_TypeDef *port, uint8_t pin, bool select);

void spi_example_jedec_id(void);
void spi_example_display_stream(const uint8_t *data, size_t len);

#endif /* SPI_H */
