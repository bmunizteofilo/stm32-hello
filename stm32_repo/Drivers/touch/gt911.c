#include "gt911.h"
#include "i2c.h"

#define GT911_REG_STATUS 0x814E
#define GT911_REG_DATA   0x8150
#define GT911_REG_CMD    0x8040

static gt911_read_f gt_read;
static gt911_write_f gt_write;
static I2C_TypeDef *gt_i2c;

#define GT911_ADDR 0x5D
#define I2C_TIMING_400KHZ 0x00300F33

static bool gt_hw_read(uint16_t reg, uint8_t *buf, uint16_t len) {
    uint8_t addr[2] = { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF) };
    return i2c_write_poll(gt_i2c, GT911_ADDR, addr, 2) &&
           i2c_read_poll(gt_i2c, GT911_ADDR, buf, len);
}

static bool gt_hw_write(uint16_t reg, const uint8_t *buf, uint16_t len) {
    uint8_t tmp[10];
    if (len > 8) return false;
    tmp[0] = (uint8_t)(reg >> 8);
    tmp[1] = (uint8_t)(reg & 0xFF);
    for (uint16_t i = 0; i < len; ++i) tmp[2+i] = buf[i];
    return i2c_write_poll(gt_i2c, GT911_ADDR, tmp, (size_t)(len + 2));
}

bool gt911_init(gt911_read_f read_f, gt911_write_f write_f) {
    if (!read_f || !write_f) return false;
    gt_read = read_f;
    gt_write = write_f;
    uint8_t cmd = 0x02; /* soft reset */
    gt_write(GT911_REG_CMD, &cmd, 1);
    return true;
}

bool gt911_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !gt_read || !gt_write) return false;
    uint8_t status;
    if (!gt_read(GT911_REG_STATUS, &status, 1)) return false;
    if ((status & 0x80) == 0 || (status & 0x0F) == 0) return false;
    uint8_t buf[4];
    if (!gt_read(GT911_REG_DATA, buf, sizeof(buf))) return false;
    uint8_t clr = 0; /* clear status */
    gt_write(GT911_REG_STATUS, &clr, 1);
    *x = (uint16_t)(buf[1] << 8 | buf[0]);
    *y = (uint16_t)(buf[3] << 8 | buf[2]);
    return true;
}

bool gt911_init_i2c(I2C_TypeDef *i2c) {
    if (!i2c) return false;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = I2C_TIMING_400KHZ,
        .own_address = 0,
    };
    if (!i2c_init(i2c, &cfg)) return false;
    i2c_enable(i2c, true);
    gt_i2c = i2c;
    return gt911_init(gt_hw_read, gt_hw_write);
}

void gt911_example(void) {
    uint16_t x, y;
    if (gt911_init_i2c(I2C1) && gt911_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

