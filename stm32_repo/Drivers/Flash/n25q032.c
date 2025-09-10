#include "n25q032.h"

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
static void n25q032_send_addr(n25q032_t *flash, uint32_t addr) {
    spi_transfer(flash->spi, (uint8_t)(addr >> 16));
    spi_transfer(flash->spi, (uint8_t)(addr >> 8));
    spi_transfer(flash->spi, (uint8_t)addr);
}

bool n25q032_init(n25q032_t *flash, SPI_TypeDef *spi) {
    if (!flash || !spi) {
        return false;
    }
    flash->spi = spi;
    return true;
}

void n25q032_read_id(n25q032_t *flash, uint8_t *manuf, uint16_t *dev) {
    if (!flash || !manuf || !dev) {
        return;
    }
    spi_transfer(flash->spi, CMD_RDID);
    *manuf = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    uint8_t hi = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    uint8_t lo = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    *dev = (uint16_t)hi << 8 | lo;
}

void n25q032_read(n25q032_t *flash, uint32_t addr, uint8_t *data, size_t len) {
    if (!flash || !data || !len) {
        return;
    }
    spi_transfer(flash->spi, CMD_READ);
    n25q032_send_addr(flash, addr);
    for (size_t i = 0; i < len; ++i) {
        data[i] = (uint8_t)spi_transfer(flash->spi, 0xFFu);
    }
}

void n25q032_page_program(n25q032_t *flash, uint32_t addr, const uint8_t *data, size_t len) {
    if (!flash || !data || !len || len > PAGE_SIZE) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_PP);
    n25q032_send_addr(flash, addr);
    for (size_t i = 0; i < len; ++i) {
        spi_transfer(flash->spi, data[i]);
    }
}

void n25q032_sector_erase(n25q032_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_SE);
    n25q032_send_addr(flash, addr);
}

void n25q032_block_erase_32k(n25q032_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_BE32K);
    n25q032_send_addr(flash, addr);
}

void n25q032_block_erase_64k(n25q032_t *flash, uint32_t addr) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_BE64K);
    n25q032_send_addr(flash, addr);
}

void n25q032_chip_erase(n25q032_t *flash) {
    if (!flash) {
        return;
    }
    spi_transfer(flash->spi, CMD_WREN);
    spi_transfer(flash->spi, CMD_CE);
}

void n25q032_example(void) {
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
    spi_init(SPI2, &cfg);
    spi_enable(SPI2, true);
    n25q032_t flash;
    n25q032_init(&flash, SPI2);
    uint8_t manuf; uint16_t dev;
    n25q032_read_id(&flash, &manuf, &dev);
    uint8_t buf[16];
    const uint8_t msg[16] = "Hello, Flash!\0";
    n25q032_sector_erase(&flash, 0u);
    n25q032_page_program(&flash, 0u, msg, sizeof(msg));
    n25q032_read(&flash, 0u, buf, sizeof(buf));
    (void)manuf; (void)dev; (void)buf;
}
