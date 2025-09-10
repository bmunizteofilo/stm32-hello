#include "stmpe811.h"
#include "i2c.h"

#define STMPE811_SYS_CTRL1 0x03
#define STMPE811_ADC_CTRL1 0x20
#define STMPE811_TSC_CFG   0x41
#define STMPE811_FIFO_STA  0x4B
#define STMPE811_TSC_FRACTION_Z 0x56
#define STMPE811_TSC_DATA_X 0x4D

static stmpe811_read_f st_read;
static stmpe811_write_f st_write;
static I2C_TypeDef *st_i2c;

#define STMPE811_ADDR 0x41
#define I2C_TIMING_100KHZ 0x00303D5B

static bool st_hw_read(uint8_t reg, uint8_t *buf, uint8_t len) {
    return i2c_write_poll(st_i2c, STMPE811_ADDR, &reg, 1) &&
           i2c_read_poll(st_i2c, STMPE811_ADDR, buf, len);
}

static bool st_hw_write(uint8_t reg, const uint8_t *buf, uint8_t len) {
    uint8_t tmp[8];
    if (len > 7) return false;
    tmp[0] = reg;
    for (uint8_t i = 0; i < len; ++i) tmp[1+i] = buf[i];
    return i2c_write_poll(st_i2c, STMPE811_ADDR, tmp, (uint8_t)(len + 1));
}

bool stmpe811_init(stmpe811_read_f read_f, stmpe811_write_f write_f) {
    if (!read_f || !write_f) return false;
    st_read = read_f;
    st_write = write_f;
    uint8_t val = 0x02; /* software reset */
    st_write(STMPE811_SYS_CTRL1, &val, 1);
    val = 0x01; /* enable ADC */
    st_write(STMPE811_ADC_CTRL1, &val, 1);
    val = 0x9A; /* TSC config */
    st_write(STMPE811_TSC_CFG, &val, 1);
    val = 0x01; /* clear FIFO */
    st_write(STMPE811_FIFO_STA, &val, 1);
    val = 0x00;
    st_write(STMPE811_FIFO_STA, &val, 1);
    return true;
}

bool stmpe811_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !st_read) return false;
    uint8_t buf[4];
    if (!st_read(STMPE811_TSC_DATA_X, buf, sizeof(buf))) return false;
    *x = (uint16_t)(buf[0] << 8 | buf[1]);
    *y = (uint16_t)(buf[2] << 8 | buf[3]);
    return true;
}

bool stmpe811_init_i2c(I2C_TypeDef *i2c) {
    if (!i2c) return false;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = I2C_TIMING_100KHZ,
        .own_address = 0,
    };
    if (!i2c_init(i2c, &cfg)) return false;
    i2c_enable(i2c, true);
    st_i2c = i2c;
    return stmpe811_init(st_hw_read, st_hw_write);
}

void stmpe811_example(void) {
    uint16_t x, y;
    if (stmpe811_init_i2c(I2C1) && stmpe811_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

