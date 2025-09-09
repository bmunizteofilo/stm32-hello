#ifndef SHT31_H
#define SHT31_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the SHT31 sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool sht31_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the SHT31 sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool sht31_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the SHT31 driver. */
void sht31_example(void);

#endif /* SHT31_H */
