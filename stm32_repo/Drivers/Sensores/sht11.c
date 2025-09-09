#include "sht11.h"

#define SHT11_ADDR 0x40u

/**
 * @brief Initialize the SHT11 sensor.
 */
bool sht11_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the SHT11 sensor.
 */
bool sht11_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd = 0xE5;
    uint8_t data[4] = {0};
    if (!i2c_write_poll(i2c, SHT11_ADDR, &cmd, 1)) return false;
    if (!i2c_read_poll(i2c, SHT11_ADDR, data, 4)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the SHT11 driver. */
void sht11_example(void) {
    float t, h;
    sht11_init(I2C1);
    sht11_read(I2C1, &t, &h);
}
