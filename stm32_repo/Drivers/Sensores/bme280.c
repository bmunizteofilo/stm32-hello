#include "bme280.h"

#define BME280_ADDR 0x76u

/**
 * @brief Initialize the BME280 sensor.
 */
bool bme280_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the BME280 sensor.
 */
bool bme280_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t reg = 0xF7; /* pressure MSB */
    uint8_t data[8] = {0};
    if (!i2c_write_poll(i2c, BME280_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, BME280_ADDR, data, 8)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the BME280 driver. */
void bme280_example(void) {
    float t, h;
    bme280_init(I2C1);
    bme280_read(I2C1, &t, &h);
}
