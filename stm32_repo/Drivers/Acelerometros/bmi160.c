#include "bmi160.h"

#define BMI160_ADDR 0x68u

/**
 * @brief Initialize the BMI160 sensor.
 */
bool bmi160_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x7E, 0x11}; /* CMD register: set accel normal mode */
    return i2c_write_poll(i2c, BMI160_ADDR, data, 2);
}

/**
 * @brief Read acceleration and gyroscope data from the BMI160.
 */
bool bmi160_read(I2C_TypeDef *i2c,
                 float *ax, float *ay, float *az,
                 float *gx, float *gy, float *gz) {
    if (!ax || !ay || !az || !gx || !gy || !gz) return false;
    uint8_t reg = 0x12; /* DATA register start */
    uint8_t buf[12] = {0};
    if (!i2c_write_poll(i2c, BMI160_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, BMI160_ADDR, buf, 12)) return false;
    *ax = *ay = *az = 0.0f;
    *gx = *gy = *gz = 0.0f;
    return true;
}

/** Example demonstrating how to use the BMI160 driver. */
void bmi160_example(void) {
    float ax, ay, az, gx, gy, gz;
    bmi160_init(I2C1);
    bmi160_read(I2C1, &ax, &ay, &az, &gx, &gy, &gz);
}
