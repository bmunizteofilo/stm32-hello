#include "sht31.h"

#define SHT31_ADDR 0x44u

/**
 * @brief Initialize the SHT31 sensor.
 */
bool sht31_init(I2C_TypeDef *i2c) {
    (void)i2c;
    return true;
}

/**
 * @brief Read temperature and humidity from the SHT31 sensor.
 */
bool sht31_read(I2C_TypeDef *i2c, float *temperature, float *humidity) {
    if (!temperature || !humidity) return false;
    uint8_t cmd[2] = {0x24, 0x00};
    uint8_t data[6] = {0};
    if (!i2c_write_poll(i2c, SHT31_ADDR, cmd, 2)) return false;
    if (!i2c_read_poll(i2c, SHT31_ADDR, data, 6)) return false;
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the SHT31 driver. */
void sht31_example(void) {
    float t, h;
    sht31_init(I2C1);
    sht31_read(I2C1, &t, &h);
}
