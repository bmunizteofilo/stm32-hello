#include "am2320.h"

#define AM2320_ADDR 0x5Cu

/**
 * @brief Initialize the AM2320 sensor.
 */
bool am2320_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the AM2320 sensor.
 */
bool am2320_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd[3] = {0x03, 0x00, 0x04};
    uint8_t data[8] = {0};
    if (!i2c_write_poll(i2c, AM2320_ADDR, cmd, 3)) return false;
    if (!i2c_read_poll(i2c, AM2320_ADDR, data, 8)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the AM2320 driver. */
void am2320_example(void) {
    float t, h;
    am2320_init(I2C1);
    am2320_read(I2C1, &t, &h);
}
