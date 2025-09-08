#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration for host builds */
typedef struct GPIO_TypeDef GPIO_TypeDef;

enum gpio_edge_t {
    GPIO_EDGE_NONE,
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING,
    GPIO_EDGE_BOTH,
};

typedef void (*gpio_cb_t)(void *ctx, uint8_t pin);

void gpio_init_output(GPIO_TypeDef *port, uint8_t pin, bool open_drain,
                      bool pull_up, bool pull_down, uint32_t speed);
void gpio_init_input(GPIO_TypeDef *port, uint8_t pin, bool pull_up,
                     bool pull_down);
void gpio_deinit(GPIO_TypeDef *port, uint8_t pin);
void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t level);
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin);
uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin);

bool gpio_attach_irq(GPIO_TypeDef *port, uint8_t pin, enum gpio_edge_t edge,
                     gpio_cb_t cb, void *ctx);
void gpio_detach_irq(GPIO_TypeDef *port, uint8_t pin);

/* Example usage:
 *   gpio_init_input(GPIOA, 0, true, false);
 *   gpio_attach_irq(GPIOA, 0, GPIO_EDGE_RISING, my_cb, NULL);
 */

#endif /* GPIO_H */
