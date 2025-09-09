#ifndef DHT22_H
#define DHT22_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/**
 * @brief Initialize DHT22 sensor on a GPIO pin.
 * @param port GPIO port used to communicate with the sensor.
 * @param pin  GPIO pin number.
 * @return true on success.
 */
bool dht22_init(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Read temperature and humidity from the DHT22 sensor.
 * @param port GPIO port used by the sensor.
 * @param pin  GPIO pin number.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool dht22_read(GPIO_TypeDef *port, uint8_t pin, float *temperature, float *humidity);

/** Example demonstrating how to use the DHT22 driver. */
void dht22_example(void);

#endif /* DHT22_H */
