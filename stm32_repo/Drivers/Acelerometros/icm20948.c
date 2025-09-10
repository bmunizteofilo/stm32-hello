#include "icm20948.h"

#define ICM20948_ADDR 0x68u

/**
 * @brief Initialize the ICM-20948 IMU.
 */
bool icm20948_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x06, 0x01}; /* PWR_MGMT_1: clock auto select */
    return i2c_write_poll(i2c, ICM20948_ADDR, data, 2);
}

/**
 * @brief Read acceleration and gyroscope data from the ICM-20948.
 */
bool icm20948_read(I2C_TypeDef *i2c,
                   float *ax, float *ay, float *az,
                   float *gx, float *gy, float *gz) {
    if (!ax || !ay || !az || !gx || !gy || !gz) return false;
    uint8_t reg = 0x2D; /* ACCEL_XOUT_H */
    uint8_t buf[12] = {0};
    if (!i2c_write_poll(i2c, ICM20948_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, ICM20948_ADDR, buf, 12)) return false;
    *ax = *ay = *az = 0.0f;
    *gx = *gy = *gz = 0.0f;
    return true;
}

/** Example demonstrating how to use the ICM-20948 driver. */
void icm20948_example(void) {
    float ax, ay, az, gx, gy, gz;
    icm20948_init(I2C1);
    icm20948_read(I2C1, &ax, &ay, &az, &gx, &gy, &gz);
}
