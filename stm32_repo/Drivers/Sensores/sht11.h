#ifndef SHT11_H
#define SHT11_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the SHT11 sensor.
 * @param i2c I2C-like peripheral to use.
 * @return true on success.
 */
bool sht11_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the SHT11 sensor.
 * @param i2c I2C-like peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool sht11_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the SHT11 driver. */
void sht11_example(void);

#endif /* SHT11_H */
