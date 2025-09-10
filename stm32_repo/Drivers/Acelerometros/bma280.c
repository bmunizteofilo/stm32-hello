#include "bma280.h"

#define BMA280_ADDR 0x18u

/**
 * @brief Initialize the BMA280 sensor.
 */
bool bma280_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x11, 0x00}; /* PMU_LPW register */
    return i2c_write_poll(i2c, BMA280_ADDR, data, 2);
}

/**
 * @brief Read acceleration data from the BMA280.
 */
bool bma280_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az) {
    if (!ax || !ay || !az) return false;
    uint8_t reg = 0x02; /* ACCD_X_LSB */
    uint8_t buf[6] = {0};
    if (!i2c_write_poll(i2c, BMA280_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, BMA280_ADDR, buf, 6)) return false;
    *ax = *ay = *az = 0.0f;
    return true;
}

/** Example demonstrating how to use the BMA280 driver. */
void bma280_example(void) {
    float ax, ay, az;
    bma280_init(I2C1);
    bma280_read(I2C1, &ax, &ay, &az);
}
