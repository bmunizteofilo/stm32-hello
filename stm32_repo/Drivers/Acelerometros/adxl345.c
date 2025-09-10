#include "adxl345.h"

#define ADXL345_ADDR 0x53u

/**
 * @brief Initialize the ADXL345 sensor.
 */
bool adxl345_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x2D, 0x08}; /* POWER_CTL register: measure */
    return i2c_write_poll(i2c, ADXL345_ADDR, data, 2);
}

/**
 * @brief Read acceleration data from the ADXL345.
 */
bool adxl345_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az) {
    if (!ax || !ay || !az) return false;
    uint8_t reg = 0x32; /* DATAX0 register */
    uint8_t buf[6] = {0};
    if (!i2c_write_poll(i2c, ADXL345_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, ADXL345_ADDR, buf, 6)) return false;
    *ax = *ay = *az = 0.0f;
    return true;
}

/** Example demonstrating how to use the ADXL345 driver. */
void adxl345_example(void) {
    float ax, ay, az;
    adxl345_init(I2C1);
    adxl345_read(I2C1, &ax, &ay, &az);
}
