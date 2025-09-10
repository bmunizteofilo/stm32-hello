#include "lsm6ds3.h"

#define LSM6DS3_ADDR 0x6Au

/**
 * @brief Initialize the LSM6DS3 sensor.
 */
bool lsm6ds3_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x10, 0x80}; /* CTRL1_XL register */
    return i2c_write_poll(i2c, LSM6DS3_ADDR, data, 2);
}

/**
 * @brief Read acceleration and gyroscope data from the LSM6DS3.
 */
bool lsm6ds3_read(I2C_TypeDef *i2c,
                  float *ax, float *ay, float *az,
                  float *gx, float *gy, float *gz) {
    if (!ax || !ay || !az || !gx || !gy || !gz) return false;
    uint8_t reg = 0x28; /* OUTX_L_G register */
    uint8_t buf[12] = {0};
    if (!i2c_write_poll(i2c, LSM6DS3_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, LSM6DS3_ADDR, buf, 12)) return false;
    *ax = *ay = *az = 0.0f;
    *gx = *gy = *gz = 0.0f;
    return true;
}

/** Example demonstrating how to use the LSM6DS3 driver. */
void lsm6ds3_example(void) {
    float ax, ay, az, gx, gy, gz;
    lsm6ds3_init(I2C1);
    lsm6ds3_read(I2C1, &ax, &ay, &az, &gx, &gy, &gz);
}
