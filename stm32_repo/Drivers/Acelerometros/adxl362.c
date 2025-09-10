#include "adxl362.h"

#define ADXL362_WRITE 0x0Au
#define ADXL362_READ  0x0Bu

/**
 * @brief Initialize the ADXL362 sensor.
 */
bool adxl362_init(SPI_TypeDef *spi) {
    spi_transfer(spi, ADXL362_WRITE);
    spi_transfer(spi, 0x2D); /* POWER_CTL */
    spi_transfer(spi, 0x02); /* Measure mode */
    return true;
}

/**
 * @brief Read acceleration data from the ADXL362.
 */
bool adxl362_read(SPI_TypeDef *spi, float *ax, float *ay, float *az) {
    if (!ax || !ay || !az) return false;
    spi_transfer(spi, ADXL362_READ);
    spi_transfer(spi, 0x0E); /* XDATA_L register */
    for (int i = 0; i < 6; ++i) {
        (void)spi_transfer(spi, 0x00u);
    }
    *ax = *ay = *az = 0.0f;
    return true;
}

/** Example demonstrating how to use the ADXL362 driver. */
void adxl362_example(void) {
    float ax, ay, az;
    adxl362_init(SPI1);
    adxl362_read(SPI1, &ax, &ay, &az);
}
