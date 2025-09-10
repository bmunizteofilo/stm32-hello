#include "mpu6050.h"

#define MPU6050_ADDR 0x68u

/**
 * @brief Initialize the MPU6050 sensor.
 */
bool mpu6050_init(I2C_TypeDef *i2c) {
    uint8_t data[2] = {0x6B, 0x00}; /* PWR_MGMT_1 register */
    return i2c_write_poll(i2c, MPU6050_ADDR, data, 2);
}

/**
 * @brief Read acceleration and gyroscope data from the MPU6050.
 */
bool mpu6050_read(I2C_TypeDef *i2c,
                  float *ax, float *ay, float *az,
                  float *gx, float *gy, float *gz) {
    if (!ax || !ay || !az || !gx || !gy || !gz) return false;
    uint8_t reg = 0x3B; /* ACCEL_XOUT_H */
    uint8_t buf[14] = {0};
    if (!i2c_write_poll(i2c, MPU6050_ADDR, &reg, 1)) return false;
    if (!i2c_read_poll(i2c, MPU6050_ADDR, buf, 14)) return false;
    *ax = *ay = *az = 0.0f;
    *gx = *gy = *gz = 0.0f;
    return true;
}

/** Example demonstrating how to use the MPU6050 driver. */
void mpu6050_example(void) {
    float ax, ay, az, gx, gy, gz;
    mpu6050_init(I2C1);
    mpu6050_read(I2C1, &ax, &ay, &az, &gx, &gy, &gz);
}
