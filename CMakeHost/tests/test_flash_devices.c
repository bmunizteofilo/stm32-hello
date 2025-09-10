#include <assert.h>
#include "spi.h"
#include "Flash/w25q64.h"
#include "Flash/mx25l128.h"
#include "Flash/at25sf041.h"
#include "Flash/n25q032.h"
#include "Flash/s25fl064l.h"
#include "Flash/gd25q32.h"
#include "Flash/en25q64.h"
#include "Flash/sst26vf064b.h"
#include "Flash/is25lp128.h"
#include "Flash/a25lq64.h"

#define TEST_INIT(name) do { \
    name##_t dev; \
    assert(name##_init(&dev, SPI1)); \
} while(0)

int main(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
        .crc_enable = false,
        .crc_poly = 0u,
    };
    spi_init(SPI1, &cfg);
    spi_enable(SPI1, true);

    TEST_INIT(w25q64);
    TEST_INIT(mx25l128);
    TEST_INIT(at25sf041);
    TEST_INIT(n25q032);
    TEST_INIT(s25fl064l);
    TEST_INIT(gd25q32);
    TEST_INIT(en25q64);
    TEST_INIT(sst26vf064b);
    TEST_INIT(is25lp128);
    TEST_INIT(a25lq64);

    w25q64_t dev; 
    w25q64_init(&dev, SPI1);
    spi_stub_reset();
    w25q64_block_erase_64k(&dev, 0);
    assert(spi_stub_get(0) == 0x06 && spi_stub_get(1) == 0xD8);
    spi_stub_reset();
    w25q64_chip_erase(&dev);
    assert(spi_stub_get(0) == 0x06 && spi_stub_get(1) == 0xC7);
    return 0;
}
