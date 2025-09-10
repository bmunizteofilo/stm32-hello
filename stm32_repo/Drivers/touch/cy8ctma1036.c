#include "cy8ctma1036.h"
#include "i2c.h"

#define CY8_REG_STATUS 0x00
#define CY8_REG_XH     0x01

static cy8ctma1036_read_f cy8_read;
static cy8ctma1036_write_f cy8_write;
static I2C_TypeDef *cy8_i2c;

#define CY8_ADDR 0x24
#define I2C_TIMING_400KHZ 0x00300F33

static bool cy8_hw_read(uint8_t reg, uint8_t *buf, uint8_t len) {
    return i2c_write_poll(cy8_i2c, CY8_ADDR, &reg, 1) &&
           i2c_read_poll(cy8_i2c, CY8_ADDR, buf, len);
}

static bool cy8_hw_write(uint8_t reg, const uint8_t *buf, uint8_t len) {
    uint8_t tmp[8];
    if (len > 7) return false;
    tmp[0] = reg;
    for (uint8_t i = 0; i < len; ++i) tmp[1+i] = buf[i];
    return i2c_write_poll(cy8_i2c, CY8_ADDR, tmp, (uint8_t)(len + 1));
}

bool cy8ctma1036_init(cy8ctma1036_read_f read_f,
                      cy8ctma1036_write_f write_f) {
    if (!read_f || !write_f) return false;
    cy8_read = read_f;
    cy8_write = write_f;
    return true;
}

bool cy8ctma1036_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !cy8_read) return false;
    uint8_t buf[5];
    if (!cy8_read(CY8_REG_STATUS, buf, sizeof(buf))) return false;
    if ((buf[0] & 0x01) == 0) return false;
    uint16_t x_val = ((buf[1] & 0x0F) << 8) | buf[2];
    uint16_t y_val = ((buf[3] & 0x0F) << 8) | buf[4];
    *x = x_val;
    *y = y_val;
    return true;
}

bool cy8ctma1036_init_i2c(I2C_TypeDef *i2c) {
    if (!i2c) return false;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = I2C_TIMING_400KHZ,
        .own_address = 0,
    };
    if (!i2c_init(i2c, &cfg)) return false;
    i2c_enable(i2c, true);
    cy8_i2c = i2c;
    return cy8ctma1036_init(cy8_hw_read, cy8_hw_write);
}

void cy8ctma1036_example(void) {
    uint16_t x, y;
    if (cy8ctma1036_init_i2c(I2C1) && cy8ctma1036_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

