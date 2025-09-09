#include "dht22.h"

/**
 * @brief Initialize DHT22 sensor on a GPIO pin.
 */
bool dht22_init(GPIO_TypeDef *port, uint8_t pin) {
    if (!port) return false;
    gpio_config(port, pin, &(gpio_cfg_t){ .mode = GPIO_MODE_OUTPUT });
    gpio_write(port, pin, 1);
    return true;
}

/**
 * @brief Read temperature and humidity from the DHT22 sensor.
 */
bool dht22_read(GPIO_TypeDef *port, uint8_t pin, float *temperature, float *humidity) {
    if (!port || !temperature || !humidity) return false;
    (void)pin; /* Stub implementation */
    *temperature = 0.0f;
    *humidity = 0.0f;
    return true;
}

/** Example demonstrating how to use the DHT22 driver. */
void dht22_example(void) {
    float t, h;
    dht22_init(GPIOA, 1);
    dht22_read(GPIOA, 1, &t, &h);
}
