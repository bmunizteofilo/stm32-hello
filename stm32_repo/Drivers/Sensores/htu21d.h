#ifndef HTU21D_H
#define HTU21D_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

/**
 * @brief Initialize the HTU21D sensor.
 * @param i2c I2C peripheral to use.
 * @return true on success.
 */
bool htu21d_init(I2C_TypeDef *i2c);

/**
 * @brief Read temperature and humidity from the HTU21D sensor.
 * @param i2c I2C peripheral.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool htu21d_read(I2C_TypeDef *i2c, float *temperature, float *humidity);

/** Example demonstrating how to use the HTU21D driver. */
void htu21d_example(void);

#endif /* HTU21D_H */
