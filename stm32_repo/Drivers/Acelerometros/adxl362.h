#ifndef ADXL362_H
#define ADXL362_H

#include <stdbool.h>
#include <stdint.h>
#include "spi.h"

/**
 * @brief Initialize the ADXL362 sensor.
 * @param spi SPI peripheral to use.
 * @return true on success.
 */
bool adxl362_init(SPI_TypeDef *spi);

/**
 * @brief Read acceleration data from the ADXL362.
 * @param spi SPI peripheral.
 * @param ax Acceleration X in g.
 * @param ay Acceleration Y in g.
 * @param az Acceleration Z in g.
 * @return true on success.
 */
bool adxl362_read(SPI_TypeDef *spi, float *ax, float *ay, float *az);

/** Example demonstrating how to use the ADXL362 driver. */
void adxl362_example(void);

#endif /* ADXL362_H */
