#ifndef SHT21_H
#define SHT21_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the SHT21 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool sht21_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the SHT21 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool sht21_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the SHT21 driver. */
void sht21_example(void);

#endif /* SHT21_H */
