#include "sdcard.h"

#define CMD0   0x40u
#define CMD8   0x48u
#define CMD17  0x51u
#define CMD24  0x58u
#define CMD55  0x77u
#define ACMD41 0x69u

static void sdcard_cs(sdcard_t *sd, bool select) {
    if (sd->cs_port) {
        spi_cs_select(sd->cs_port, sd->cs_pin, select);
    }
}

bool sdcard_init(sdcard_t *sd, SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin) {
    if (!sd || !spi) {
        return false;
    }
    sd->spi = spi;
    sd->cs_port = cs_port;
    sd->cs_pin = cs_pin;
    if (cs_port) {
        spi_cs_init(cs_port, cs_pin);
        sdcard_cs(sd, false);
    }
    for (int i = 0; i < 10; ++i) {
        spi_transfer(spi, 0xFFu);
    }
    sdcard_cs(sd, true);
    spi_transfer(spi, CMD0);            /* GO_IDLE_STATE */
    spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0);
    spi_transfer(spi, 0x95u);
    spi_transfer(spi, 0xFFu);
    spi_transfer(spi, CMD8);            /* SEND_IF_COND */
    spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0x01u); spi_transfer(spi, 0xAAu);
    spi_transfer(spi, 0x87u);
    for (int i = 0; i < 5; ++i) {
        spi_transfer(spi, 0xFFu);
    }
    spi_transfer(spi, CMD55);           /* APP_CMD */
    spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0);
    spi_transfer(spi, 0xFFu);
    spi_transfer(spi, 0xFFu);
    spi_transfer(spi, ACMD41);          /* SD_SEND_OP_COND */
    spi_transfer(spi, 0x40u); spi_transfer(spi, 0); spi_transfer(spi, 0); spi_transfer(spi, 0);
    spi_transfer(spi, 0xFFu);
    spi_transfer(spi, 0xFFu);
    sdcard_cs(sd, false);
    return true;
}

bool sdcard_read_block(sdcard_t *sd, uint32_t addr, uint8_t *buf) {
    if (!sd || !buf) {
        return false;
    }
    sdcard_cs(sd, true);
    spi_transfer(sd->spi, CMD17);
    spi_transfer(sd->spi, (uint8_t)(addr >> 24));
    spi_transfer(sd->spi, (uint8_t)(addr >> 16));
    spi_transfer(sd->spi, (uint8_t)(addr >> 8));
    spi_transfer(sd->spi, (uint8_t)addr);
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFFu);
    for (size_t i = 0; i < 512u; ++i) {
        buf[i] = (uint8_t)spi_transfer(sd->spi, 0xFFu);
    }
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFFu);
    sdcard_cs(sd, false);
    return true;
}

bool sdcard_write_block(sdcard_t *sd, uint32_t addr, const uint8_t *buf) {
    if (!sd || !buf) {
        return false;
    }
    sdcard_cs(sd, true);
    spi_transfer(sd->spi, CMD24);
    spi_transfer(sd->spi, (uint8_t)(addr >> 24));
    spi_transfer(sd->spi, (uint8_t)(addr >> 16));
    spi_transfer(sd->spi, (uint8_t)(addr >> 8));
    spi_transfer(sd->spi, (uint8_t)addr);
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFEu);
    for (size_t i = 0; i < 512u; ++i) {
        spi_transfer(sd->spi, buf[i]);
    }
    spi_transfer(sd->spi, 0xFFu);
    spi_transfer(sd->spi, 0xFFu);
    sdcard_cs(sd, false);
    return true;
}

void sdcard_example(void) {
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8u,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
        .baudrate = SPI_BAUD_DIV8,
        .crc_enable = false,
        .crc_poly = 0u,
    };
    spi_init(SPI1, &cfg);
    spi_enable(SPI1, true);
    sdcard_t card;
    sdcard_init(&card, SPI1, NULL, 0u);
    uint8_t buffer[512];
    sdcard_read_block(&card, 0u, buffer);
    buffer[0] = 'S'; buffer[1] = 'D';
    sdcard_write_block(&card, 0u, buffer);
}
