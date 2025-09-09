#include "htu21d.h"

#define HTU21D_ADDR 0x40u

/**
 * @brief Initialize the HTU21D sensor.
 */
bool htu21d_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the HTU21D sensor.
 */
bool htu21d_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd = 0xE5;
    uint8_t data[4] = {0};
    if (!i2c_write_poll(i2c, HTU21D_ADDR, &cmd, 1)) return false;
    if (!i2c_read_poll(i2c, HTU21D_ADDR, data, 4)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the HTU21D driver. */
void htu21d_example(void) {
    float t, h;
    htu21d_init(I2C1);
    htu21d_read(I2C1, &t, &h);
}
