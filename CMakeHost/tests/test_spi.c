#include <assert.h>
#include "spi.h"
#include "dma.h"

int main(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
    };
    assert(spi_init((SPI_TypeDef *)0x40013000u, &cfg));
    cfg.datasize = 3u;
    assert(!spi_init((SPI_TypeDef *)0x40013000u, &cfg));
    cfg.datasize = 17u;
    assert(!spi_init((SPI_TypeDef *)0x40013000u, &cfg));
    DMA_Channel_TypeDef ch;
    assert(!spi_transfer_it_start((SPI_TypeDef *)0x40013000u, NULL, NULL, 1, NULL, NULL));
    assert(!spi_transfer_dma_start((SPI_TypeDef *)0x40013000u, &ch, &ch, NULL, NULL, 1, NULL, NULL));
    return 0;
}
