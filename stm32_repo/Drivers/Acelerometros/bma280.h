#ifndef BMA280_H
#define BMA280_H

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

/**
 * @brief Initialize the BMA280 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool bma280_init(I2C_TypeDef *i2c);

/**
 * @brief Read acceleration data from the BMA280.
 * @param i2c I2C peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @return true on success.
 */
bool bma280_read(I2C_TypeDef *i2c, float *ax, float *ay, float *az);

/** Example demonstrating how to use the BMA280 driver. */
void bma280_example(void);

#endif /* BMA280_H */
