#include <assert.h>
#include <stdint.h>

#include "spi.h"
#include "touch/xpt2046.h"
#include "touch/ads7843.h"
#include "touch/ft6206.h"
#include "touch/ft6236.h"
#include "touch/gt911.h"
#include "touch/tsc2007.h"
#include "touch/stmpe811.h"
#include "touch/ft5406.h"
#include "touch/ar1021.h"
#include "touch/cy8ctma1036.h"

/* SPI-based controllers */
static uint16_t spi_transfer_stub(uint8_t cmd) {
    switch (cmd) {
    case 0xD0: /* X */
        return (uint16_t)(100 << 3);
    case 0x90: /* Y */
        return (uint16_t)(200 << 3);
    default:
        return 0;
    }
}

static uint16_t tsc2007_read_stub(uint8_t cmd) {
    if (cmd == 0xC0) return (uint16_t)(100 << 4);
    if (cmd == 0xD0) return (uint16_t)(200 << 4);
    return 0;
}

/* I2C-based simple controllers (FT6206 family and CY8) */
static bool i2c_read_5_stub(uint8_t reg, uint8_t *buf, uint8_t len) {
    (void)reg;
    if (len == 5) {
        buf[0] = 1;
        buf[1] = (uint8_t)((100 >> 8) & 0x0F);
        buf[2] = (uint8_t)(100 & 0xFF);
        buf[3] = (uint8_t)((200 >> 8) & 0x0F);
        buf[4] = (uint8_t)(200 & 0xFF);
    }
    return true;
}

static bool i2c_write_ignore(uint8_t reg, const uint8_t *buf, uint8_t len) {
    (void)reg; (void)buf; (void)len; return true;
}

/* GT911 specific stubs */
static bool gt_read_stub(uint16_t reg, uint8_t *buf, uint16_t len) {
    if (reg == 0x814E && len >= 1) {
        buf[0] = 0x81; /* data ready, 1 touch */
    } else if (reg == 0x8150 && len >= 4) {
        buf[0] = (uint8_t)(100 & 0xFF);
        buf[1] = (uint8_t)(100 >> 8);
        buf[2] = (uint8_t)(200 & 0xFF);
        buf[3] = (uint8_t)(200 >> 8);
    }
    return true;
}

static bool gt_write_stub(uint16_t reg, const uint8_t *buf, uint16_t len) {
    (void)reg; (void)buf; (void)len; return true;
}

/* STMPE811 stubs */
static bool stmpe_read_stub(uint8_t reg, uint8_t *buf, uint8_t len) {
    if (reg == 0x4D && len >= 4) {
        buf[0] = (uint8_t)(100 >> 8);
        buf[1] = (uint8_t)(100 & 0xFF);
        buf[2] = (uint8_t)(200 >> 8);
        buf[3] = (uint8_t)(200 & 0xFF);
    }
    return true;
}

static bool stmpe_write_stub(uint8_t reg, const uint8_t *buf, uint8_t len) {
    (void)reg; (void)buf; (void)len; return true;
}

int main(void) {
    uint16_t x = 0, y = 0;

    assert(xpt2046_init_spi(SPI1, NULL, 0));
    assert(xpt2046_init(spi_transfer_stub));
    assert(xpt2046_read(&x, &y) && x == 100 && y == 200);

    assert(ads7843_init_spi(SPI1, NULL, 0));
    assert(ads7843_init(spi_transfer_stub));
    assert(ads7843_read(&x, &y) && x == 100 && y == 200);

    assert(ar1021_init_spi(SPI1, NULL, 0));
    assert(ar1021_init(spi_transfer_stub));
    assert(ar1021_read(&x, &y) && x == 100 && y == 200);

    assert(tsc2007_init(tsc2007_read_stub));
    assert(tsc2007_read(&x, &y) && x == 100 && y == 200);

    assert(ft6206_init(i2c_read_5_stub, i2c_write_ignore));
    assert(ft6206_read(&x, &y) && x == 100 && y == 200);

    assert(ft6236_init(i2c_read_5_stub, i2c_write_ignore));
    assert(ft6236_read(&x, &y) && x == 100 && y == 200);

    assert(ft5406_init(i2c_read_5_stub, i2c_write_ignore));
    assert(ft5406_read(&x, &y) && x == 100 && y == 200);

    assert(cy8ctma1036_init(i2c_read_5_stub, i2c_write_ignore));
    assert(cy8ctma1036_read(&x, &y) && x == 100 && y == 200);

    assert(gt911_init(gt_read_stub, gt_write_stub));
    assert(gt911_read(&x, &y) && x == 100 && y == 200);

    assert(stmpe811_init(stmpe_read_stub, stmpe_write_stub));
    assert(stmpe811_read(&x, &y) && x == 100 && y == 200);

    return 0;
}

