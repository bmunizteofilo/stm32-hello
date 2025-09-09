#include <assert.h>
#include "spi.h"
#include "dma.h"
#include "rcc.h"

RCC_TypeDef rcc_regs;
FLASH_TypeDef flash_regs;

int main(void) {
    volatile uint32_t spi_mem[9] = {0};
    SPI_TypeDef *spi = (SPI_TypeDef *)spi_mem;
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    assert(!spi_init(spi, &cfg));
    cfg.datasize = 3u;
    assert(!spi_init(spi, &cfg));
    cfg.datasize = 17u;
    assert(!spi_init(spi, &cfg));
    DMA_Channel_TypeDef ch;
    assert(!spi_transfer_it_start(spi, NULL, NULL, 1, NULL, NULL));
    assert(!spi_transfer_dma_start(spi, &ch, &ch, NULL, NULL, 1, NULL, NULL));
    return 0;
}
