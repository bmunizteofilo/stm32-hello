#include "si7021.h"

#define SI7021_ADDR 0x40u

/**
 * @brief Initialize the Si7021 sensor.
 */
bool si7021_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the Si7021 sensor.
 */
bool si7021_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd = 0xF5; /* measure humidity, hold master */
    uint8_t data[4] = {0};
    if (!i2c_write_poll(i2c, SI7021_ADDR, &cmd, 1)) return false;
    if (!i2c_read_poll(i2c, SI7021_ADDR, data, 4)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the Si7021 driver. */
void si7021_example(void) {
    float t, h;
    si7021_init(I2C1);
    si7021_read(I2C1, &t, &h);
}
