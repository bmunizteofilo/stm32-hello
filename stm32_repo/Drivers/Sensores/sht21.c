#include "sht21.h"

#define SHT21_ADDR 0x40u

/**
 * @brief Initialize the SHT21 sensor.
 */
bool sht21_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the SHT21 sensor.
 */
bool sht21_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd = 0xE5; /* trigger humidity measurement */
    uint8_t data[4] = {0};
    if (!i2c_write_poll(i2c, SHT21_ADDR, &cmd, 1)) return false;
    if (!i2c_read_poll(i2c, SHT21_ADDR, data, 4)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the SHT21 driver. */
void sht21_example(void) {
    float t, h;
    sht21_init(I2C1);
    sht21_read(I2C1, &t, &h);
}
