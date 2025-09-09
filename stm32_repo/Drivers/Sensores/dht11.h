#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/**
 * @brief Initialize DHT11 sensor on a GPIO pin.
 * @param port GPIO port used to communicate with the sensor.
 * @param pin  GPIO pin number.
 * @return true on success.
 */
bool dht11_init(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Read temperature and humidity from the DHT11 sensor.
 * @param port GPIO port used by the sensor.
 * @param pin  GPIO pin number.
 * @param temperature Output temperature in Celsius.
 * @param humidity    Output relative humidity percentage.
 * @return true on success.
 */
bool dht11_read(GPIO_TypeDef *port, uint8_t pin, float *temperature, float *humidity);

/** Example demonstrating how to use the DHT11 driver. */
void dht11_example(void);

#endif /* DHT11_H */
