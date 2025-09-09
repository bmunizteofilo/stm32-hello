#ifndef AM2320_H
#define AM2320_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the AM2320 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool am2320_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the AM2320 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool am2320_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the AM2320 driver. */
void am2320_example(void);

#endif /* AM2320_H */
