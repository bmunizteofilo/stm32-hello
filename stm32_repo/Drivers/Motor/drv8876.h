#ifndef DRV8876_H
#define DRV8876_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8876.h
 * @brief Driver para o CI DRV8876 (ponte H simples com controle de corrente).
 *
 * Exemplos de aplicação reais:
 * 1. Atuadores de freio eletromecânico.
 * 2. Válvulas de controle proporcional.
 * 3. Veículos de brinquedo com reversão.
 */

typedef void (*drv8876_gpio_write_t)(bool level);
typedef void (*drv8876_pwm_write_t)(uint8_t duty);

typedef struct {
    drv8876_gpio_write_t in1;
    drv8876_gpio_write_t in2;
    drv8876_pwm_write_t pwm;
    drv8876_gpio_write_t sleep; /* opcional */
} drv8876_pins_t;

typedef struct {
    drv8876_pins_t pins;
} drv8876_t;

enum drv8876_state {
    DRV8876_COAST,
    DRV8876_FORWARD,
    DRV8876_REVERSE,
    DRV8876_BRAKE,
};

void drv8876_init(drv8876_t *dev, const drv8876_pins_t *pins);
void drv8876_set(drv8876_t *dev, enum drv8876_state st);
void drv8876_set_speed(drv8876_t *dev, uint8_t duty);
void drv8876_sleep(drv8876_t *dev, bool sleep);

#endif // DRV8876_H
