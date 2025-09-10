#include "s25fl064l.h"

#define CMD_READ   0x03u
#define CMD_PP     0x02u
#define CMD_SE     0x20u
#define CMD_WREN   0x06u
#define CMD_RDID   0x9Fu
#define CMD_BE32K  0x52u
#define CMD_BE64K  0xD8u
#define CMD_CE     0xC7u
#define PAGE_SIZE  256u

/* Send a 24-bit address MSB first. */
static void s25fl064l_send_addr(s25fl064l_t *flash, uint32_t addr) {
    spi_transfer(flash->spi, (uint8_t)(addr >> 16));
    spi_transfer(flash->spi, (uint8_t)(addr >> 8));
    spi_transfer(flash->spi, (uint8_t)addr);
}

bool s25fl064l_init(s25fl064l_t *flash, SPI_TypeDef *spi) {
    if (!flash || !spi) {
        return false;
    }
    flash->spi = spi;
    return true;
}

void s25fl064l_read_id(s25fl064l_t *flash, uint8_t *manuf, uint16_t *dev) {
    if (!flash || !manuf || !dev) {
        return;
    }
    spi_transfer(flash->spi, CMD_RDID);
    *manuf = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    uint8_t hi = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    uint8_t lo = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    *dev = (uint16_t)hi << 8 | lo;
}

void s25fl064l_read(s25fl064l_t *flash, uint32_t addr, uint8_t *data, size_t len) {
    if (!flash || !data || !len) {
        return;
    }
    spi_transfer(flash->spi, CMD_READ);
    s25fl064l_send_addr(flash, addr);
    for (size_t i = 0; i < len; ++i) {
        data[i] = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    }
}

void s25fl064l_page_program(s25fl064l_t *flash, uint32_t addr, const uint8_t *data, size_t len) {
    if (!flash || !data || !len || len > PAGE_SIZE) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_PP);
    s25fl064l_send_addr(flash, addr);
    for (size_t i = 0; i < len; ++i) {
        spi_transfer(flash->spi, data[i]);
    }
}

void s25fl064l_sector_erase(s25fl064l_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_SE);
    s25fl064l_send_addr(flash, addr);
}

void s25fl064l_block_erase_32k(s25fl064l_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_BE32K);
    s25fl064l_send_addr(flash, addr);
}

void s25fl064l_block_erase_64k(s25fl064l_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_BE64K);
    s25fl064l_send_addr(flash, addr);
}

void s25fl064l_chip_erase(s25fl064l_t *flash) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_CE);
}

void s25fl064l_example(void) {
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
    s25fl064l_t flash;
    s25fl064l_init(&flash, SPI1);
    uint8_t manuf; uint16_t dev;
    s25fl064l_read_id(&flash, &manuf, &dev);
    uint8_t buf[16];
    const uint8_t msg[16] = "Hello, Flash!\0";
    s25fl064l_sector_erase(&flash, 0u);
    s25fl064l_page_program(&flash, 0u, msg, sizeof(msg));
    s25fl064l_read(&flash, 0u, buf, sizeof(buf));
    (void)manuf; (void)dev; (void)buf;
}
