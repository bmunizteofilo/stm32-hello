#include "ft6206.h"
#include "i2c.h"

#define FT6206_REG_DATA 0x02

static ft6206_read_f ft_read;
static ft6206_write_f ft_write;
static I2C_TypeDef *ft_i2c;

#define FT6206_ADDR 0x38
#define I2C_TIMING_400KHZ 0x00300F33

static bool ft_hw_read(uint8_t reg, uint8_t *buf, uint8_t len) {
    return i2c_write_poll(ft_i2c, FT6206_ADDR, &reg, 1) &&
           i2c_read_poll(ft_i2c, FT6206_ADDR, buf, len);
}

static bool ft_hw_write(uint8_t reg, const uint8_t *buf, uint8_t len) {
    uint8_t tmp[8];
    if (len > 7) return false;
    tmp[0] = reg;
    for (uint8_t i = 0; i < len; ++i) tmp[1+i] = buf[i];
    return i2c_write_poll(ft_i2c, FT6206_ADDR, tmp, (uint8_t)(len + 1));
}

bool ft6206_init(ft6206_read_f read_f, ft6206_write_f write_f) {
    if (!read_f || !write_f) return false;
    ft_read = read_f;
    ft_write = write_f;
    return true;
}

bool ft6206_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !ft_read) return false;
    uint8_t buf[5];
    if (!ft_read(FT6206_REG_DATA, buf, sizeof(buf))) return false;
    if (buf[0] == 0) return false;
    uint16_t x_val = ((buf[1] & 0x0F) << 8) | buf[2];
    uint16_t y_val = ((buf[3] & 0x0F) << 8) | buf[4];
    *x = x_val;
    *y = y_val;
    return true;
}

bool ft6206_init_i2c(I2C_TypeDef *i2c) {
    if (!i2c) return false;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = I2C_TIMING_400KHZ,
        .own_address = 0,
    };
    if (!i2c_init(i2c, &cfg)) return false;
    i2c_enable(i2c, true);
    ft_i2c = i2c;
    return ft6206_init(ft_hw_read, ft_hw_write);
}

void ft6206_example(void) {
    uint16_t x, y;
    if (ft6206_init_i2c(I2C1) && ft6206_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

