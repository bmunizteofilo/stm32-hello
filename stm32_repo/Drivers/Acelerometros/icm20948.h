#ifndef ICM20948_H
#define ICM20948_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the ICM-20948 IMU.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool icm20948_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration and gyroscope data from the ICM-20948.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @param gx Gyro X in deg/s.
 * @param gy Gyro Y in deg/s.
 * @param gz Gyro Z in deg/s.
 * @return true on success.
 */
bool icm20948_read(I2C_TypeDef *i2c,
                   float *ax, float *ay, float *az,
                   float *gx, float *gy, float *gz);

/** Example demonstrating how to use the ICM-20948 driver. */
void icm20948_example(void);

#endif /* ICM20948_H */
