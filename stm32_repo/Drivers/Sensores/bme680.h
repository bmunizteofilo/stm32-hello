#ifndef BME680_H
#define BME680_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the BME680 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool bme680_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the BME680 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool bme680_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the BME680 driver. */
void bme680_example(void);

#endif /* BME680_H */
