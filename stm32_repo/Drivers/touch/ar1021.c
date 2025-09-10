#include "ar1021.h"
#include "spi.h"
#include "gpio.h"

#define AR1021_CMD_X 0xD0
#define AR1021_CMD_Y 0x90

static ar1021_transfer_f ar1021_transfer;
static int16_t cal_x_min = 0, cal_x_max = 4095;
static int16_t cal_y_min = 0, cal_y_max = 4095;
static SPI_TypeDef *ar_spi;
static GPIO_TypeDef *ar_cs_port;
static uint8_t ar_cs_pin;

static uint16_t ar_spi_xfer(uint8_t cmd) {
    if (ar_cs_port) spi_cs_select(ar_cs_port, ar_cs_pin, true);
    uint16_t v = spi_transfer(ar_spi, cmd);
    if (ar_cs_port) spi_cs_select(ar_cs_port, ar_cs_pin, false);
    return v;
}

bool ar1021_init(ar1021_transfer_f transfer) {
    if (!transfer) return false;
    ar1021_transfer = transfer;
    cal_x_min = 0;
    cal_x_max = 4095;
    cal_y_min = 0;
    cal_y_max = 4095;
    return true;
}

void ar1021_set_calibration(int16_t x_min, int16_t x_max,
                            int16_t y_min, int16_t y_max) {
    cal_x_min = x_min;
    cal_x_max = x_max;
    cal_y_min = y_min;
    cal_y_max = y_max;
}

static uint16_t scale(int16_t val, int16_t min, int16_t max) {
    if (max == min) return 0;
    if (val < min) val = min;
    if (val > max) val = max;
    return (uint16_t)((val - min) * 4095 / (max - min));
}

bool ar1021_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !ar1021_transfer) return false;
    uint16_t raw_x = ar1021_transfer(AR1021_CMD_X) >> 3;
    uint16_t raw_y = ar1021_transfer(AR1021_CMD_Y) >> 3;
    *x = scale(raw_x, cal_x_min, cal_x_max);
    *y = scale(raw_y, cal_y_min, cal_y_max);
    return true;
}

bool ar1021_init_spi(SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin) {
    if (!spi) return false;
    spi_cfg_t cfg = {
        .mode = SPI_MODE_MASTER,
        .direction = SPI_DIR_FULL_DUPLEX,
        .cpol = SPI_CPOL_LOW,
        .cpha = SPI_CPHA_1EDGE,
        .datasize = 8,
        .first_bit = SPI_FIRST_MSB,
        .nss = SPI_NSS_SOFT,
        .baudrate = SPI_BAUD_DIV8,
        .crc_enable = false,
        .crc_poly = 7,
    };
    if (!spi_init(spi, &cfg)) return false;
    spi_enable(spi, true);
    ar_spi = spi;
    ar_cs_port = cs_port;
    ar_cs_pin = cs_pin;
    if (cs_port) spi_cs_init(cs_port, cs_pin);
    return ar1021_init(ar_spi_xfer);
}

void ar1021_example(void) {
    uint16_t x, y;
    if (ar1021_init_spi(SPI1, GPIOA, 4) && ar1021_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

