#include <assert.h>
#include "sdcard.h"
#include "spi.h"

static bool contains_cmd(uint8_t cmd) {
    for (size_t i = 0; i < spi_stub_count(); ++i) {
        if (spi_stub_get(i) == cmd) {
            return true;
        }
    }
    return false;
}

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
    sdcard_t card;
    spi_stub_reset();
    assert(sdcard_init(&card, SPI1, NULL, 0u));
    for (size_t i = 0; i < 10; ++i) {
        assert(spi_stub_get(i) == 0xFF);
    }
    assert(contains_cmd(0x40)); /* CMD0 */
    assert(contains_cmd(0x48)); /* CMD8 */
    assert(contains_cmd(0x77)); /* CMD55 */
    assert(contains_cmd(0x69)); /* ACMD41 */

    uint8_t buf[512] = {0};
    spi_stub_reset();
    assert(sdcard_read_block(&card, 0x12345678u, buf));
    assert(spi_stub_get(0) == 0x51); /* CMD17 */
    assert(spi_stub_get(1) == 0x12);
    assert(spi_stub_get(2) == 0x34);
    assert(spi_stub_get(3) == 0x56);
    assert(spi_stub_get(4) == 0x78);
    assert(spi_stub_get(5) == 0xFF);

    spi_stub_reset();
    assert(sdcard_write_block(&card, 0x12345678u, buf));
    assert(spi_stub_get(0) == 0x58); /* CMD24 */
    assert(spi_stub_get(1) == 0x12);
    assert(spi_stub_get(2) == 0x34);
    assert(spi_stub_get(3) == 0x56);
    assert(spi_stub_get(4) == 0x78);
    assert(spi_stub_get(5) == 0xFF);
    assert(spi_stub_get(7) == 0xFE); /* token */
    assert(spi_stub_count() == 256);
    return 0;
}
