#include <assert.h>
#include <string.h>
#include "spi.h"
#include "Flash/w25q64.h"

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

    w25q64_t flash;
    assert(w25q64_init(&flash, SPI1));

    uint8_t manuf; uint16_t dev;
    spi_stub_reset();
    w25q64_read_id(&flash, &manuf, &dev);
    assert(spi_stub_get(0) == 0x9F); /* RDID */
    spi_stub_reset();

    uint8_t data[4] = {1,2,3,4};
    w25q64_page_program(&flash, 0x001234u, data, sizeof(data));
    assert(spi_stub_get(0) == 0x06); /* WREN */
    assert(spi_stub_get(1) == 0x02); /* PP */
    assert(spi_stub_get(2) == 0x00 && spi_stub_get(3) == 0x12 && spi_stub_get(4) == 0x34);
    assert(spi_stub_get(5) == 1 && spi_stub_get(6) == 2 && spi_stub_get(7) == 3 && spi_stub_get(8) == 4);
    spi_stub_reset();

    w25q64_read(&flash, 0x001234u, data, 2);
    assert(spi_stub_get(0) == 0x03); /* READ */
    assert(spi_stub_get(1) == 0x00 && spi_stub_get(2) == 0x12 && spi_stub_get(3) == 0x34);
    spi_stub_reset();

    w25q64_sector_erase(&flash, 0x001000u);
    assert(spi_stub_get(0) == 0x06); /* WREN */
    assert(spi_stub_get(1) == 0x20); /* SE */
    assert(spi_stub_get(2) == 0x00 && spi_stub_get(3) == 0x10 && spi_stub_get(4) == 0x00);

    return 0;
}
