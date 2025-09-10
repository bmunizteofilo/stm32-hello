#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the MPU6050 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool mpu6050_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration and gyroscope data.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @param gx Gyro X in deg/s.
 * @param gy Gyro Y in deg/s.
 * @param gz Gyro Z in deg/s.
 * @return true on success.
 */
bool mpu6050_read(I2C_TypeDef *i2c,
                  float *ax, float *ay, float *az,
                  float *gx, float *gy, float *gz);

/** Example demonstrating how to use the MPU6050 driver. */
void mpu6050_example(void);

#endif /* MPU6050_H */
