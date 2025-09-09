#ifndef BME280_H
#define BME280_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the BME280 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool bme280_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the BME280 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool bme280_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the BME280 driver. */
void bme280_example(void);

#endif /* BME280_H */
