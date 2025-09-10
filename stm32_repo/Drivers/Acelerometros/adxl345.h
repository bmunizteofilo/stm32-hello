#ifndef ADXL345_H
#define ADXL345_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

/**
 * @brief Initialize the ADXL345 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool adxl345_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration data from the ADXL345.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @return true on success.
 */
bool adxl345_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az);

/** Example demonstrating how to use the ADXL345 driver. */
void adxl345_example(void);

#endif /* ADXL345_H */
