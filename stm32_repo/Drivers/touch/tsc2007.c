#include "tsc2007.h"
#include "i2c.h"

#define TSC2007_CMD_X 0xC0
#define TSC2007_CMD_Y 0xD0

static tsc2007_read_f tsc2007_reader;
static int16_t cal_x_min = 0, cal_x_max = 4095;
static int16_t cal_y_min = 0, cal_y_max = 4095;
static I2C_TypeDef *tsc_i2c;

#define TSC2007_ADDR 0x48
#define I2C_TIMING_400KHZ 0x00300F33

static uint16_t tsc_hw_reader(uint8_t cmd) {
    uint8_t buf[2];
    uint8_t c = cmd;
    if (!i2c_write_poll(tsc_i2c, TSC2007_ADDR, &c, 1)) return 0;
    if (!i2c_read_poll(tsc_i2c, TSC2007_ADDR, buf, 2)) return 0;
    return (uint16_t)(buf[0] << 8 | buf[1]);
}

bool tsc2007_init(tsc2007_read_f reader) {
    if (!reader) return false;
    tsc2007_reader = reader;
    cal_x_min = 0;
    cal_x_max = 4095;
    cal_y_min = 0;
    cal_y_max = 4095;
    return true;
}

void tsc2007_set_calibration(int16_t x_min, int16_t x_max,
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

bool tsc2007_read(uint16_t *x, uint16_t *y) {
    if (!x || !y || !tsc2007_reader) return false;
    uint16_t raw_x = tsc2007_reader(TSC2007_CMD_X) >> 4;
    uint16_t raw_y = tsc2007_reader(TSC2007_CMD_Y) >> 4;
    *x = scale(raw_x, cal_x_min, cal_x_max);
    *y = scale(raw_y, cal_y_min, cal_y_max);
    return true;
}

bool tsc2007_init_i2c(I2C_TypeDef *i2c) {
    if (!i2c) return false;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = I2C_TIMING_400KHZ,
        .own_address = 0,
    };
    if (!i2c_init(i2c, &cfg)) return false;
    i2c_enable(i2c, true);
    tsc_i2c = i2c;
    return tsc2007_init(tsc_hw_reader);
}

void tsc2007_example(void) {
    uint16_t x, y;
    if (tsc2007_init_i2c(I2C1) && tsc2007_read(&x, &y)) {
        (void)x;
        (void)y;
    }
}

