#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration for host builds */
typedef struct GPIO_TypeDef GPIO_TypeDef;

#if defined(STM32F0_FIRMWARE)
#define GPIOA ((GPIO_TypeDef *)0x48000000u)
#define GPIOB ((GPIO_TypeDef *)0x48000400u)
#define GPIOC ((GPIO_TypeDef *)0x48000800u)
#define GPIOD ((GPIO_TypeDef *)0x48000C00u)
#define GPIOE ((GPIO_TypeDef *)0x48001000u)
#define GPIOF ((GPIO_TypeDef *)0x48001400u)
#endif

enum gpio_edge_t {
    GPIO_EDGE_NONE,
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING,
    GPIO_EDGE_BOTH,
};

enum gpio_mode_t {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG,
};

enum gpio_pull_t {
    GPIO_PULL_NONE,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
};

enum gpio_otype_t {
    GPIO_OTYPE_PP,
    GPIO_OTYPE_OD,
};

enum gpio_speed_t {
    GPIO_SPEED_LOW,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH,
};

typedef struct {
    enum gpio_mode_t mode;
    enum gpio_pull_t pull;
    enum gpio_otype_t otype;
    enum gpio_speed_t speed;
    uint8_t alternate; /* alternate function index when mode == GPIO_MODE_AF */
} gpio_cfg_t;

typedef void (*gpio_cb_t)(void *ctx, uint8_t pin);
void gpio_config(GPIO_TypeDef *port, uint8_t pin, const gpio_cfg_t *cfg);
void gpio_deinit(GPIO_TypeDef *port, uint8_t pin);
void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t level);
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin);
uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin);

bool gpio_attach_irq(GPIO_TypeDef *port, uint8_t pin, enum gpio_edge_t edge,
                     gpio_cb_t cb, void *ctx);
void gpio_detach_irq(GPIO_TypeDef *port, uint8_t pin);

/* Convenience examples to exercise the driver from main() */
void gpio_example_output(void);
void gpio_example_input(void);
void gpio_example_irq(void);
void gpio_example_toggle(void);

/* Example usage:
 *   gpio_config(GPIOA, 0, &(gpio_cfg_t){
 *       .mode = GPIO_MODE_INPUT,
 *       .pull = GPIO_PULL_UP,
 *   });
 *   gpio_attach_irq(GPIOA, 0, GPIO_EDGE_RISING, my_cb, NULL);
 *
 *   gpio_config(GPIOA, 5, &(gpio_cfg_t){
 *       .mode = GPIO_MODE_OUTPUT,
 *       .otype = GPIO_OTYPE_PP,
 *       .speed = GPIO_SPEED_HIGH,
 *   });
 *   gpio_write(GPIOA, 5, 1);
 */

#endif /* GPIO_H */
