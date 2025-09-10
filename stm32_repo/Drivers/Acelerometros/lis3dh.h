#ifndef LIS3DH_H
#define LIS3DH_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the LIS3DH accelerometer.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool lis3dh_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration data from the LIS3DH.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @return true on success.
 */
bool lis3dh_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az);

/** Example demonstrating how to use the LIS3DH driver. */
void lis3dh_example(void);

#endif /* LIS3DH_H */
