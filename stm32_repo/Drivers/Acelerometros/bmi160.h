#ifndef BMI160_H
#define BMI160_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

/**
 * @brief Initialize the BMI160 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool bmi160_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration and gyroscope data from the BMI160.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @param gx Gyro X in deg/s.
 * @param gy Gyro Y in deg/s.
 * @param gz Gyro Z in deg/s.
 * @return true on success.
 */
bool bmi160_read(I2C_TypeDef *i2c,
                 float *ax, float *ay, float *az,
                 float *gx, float *gy, float *gz);

/** Example demonstrating how to use the BMI160 driver. */
void bmi160_example(void);

#endif /* BMI160_H */
