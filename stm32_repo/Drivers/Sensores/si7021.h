#ifndef SI7021_H
#define SI7021_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the Si7021 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool si7021_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the Si7021 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool si7021_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the Si7021 driver. */
void si7021_example(void);

#endif /* SI7021_H */
