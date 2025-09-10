#ifndef DRV8871_H
#define DRV8871_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8871.h
 * @brief Driver para o CI DRV8871 (ponte H simples de alta corrente).
 *
 * Exemplos de aplicação reais:
 * 1. Acionamento de motor de braço robótico.
 * 2. Controle de bomba de água em mini-hidrônicos.
 * 3. Condução de motor em portões deslizantes.
 */

typedef void (*drv8871_gpio_write_t)(bool level);
typedef void (*drv8871_pwm_write_t)(uint8_t duty);

typedef struct {
    drv8871_gpio_write_t in1;
    drv8871_gpio_write_t in2;
    drv8871_pwm_write_t pwm; /* aplicação de duty no IN1 */
    drv8871_gpio_write_t sleep; /* opcional */
} drv8871_pins_t;

typedef struct {
    drv8871_pins_t pins;
} drv8871_t;

enum drv8871_state {
    DRV8871_COAST,
    DRV8871_FORWARD,
    DRV8871_REVERSE,
    DRV8871_BRAKE,
};

void drv8871_init(drv8871_t *dev, const drv8871_pins_t *pins);
void drv8871_set(drv8871_t *dev, enum drv8871_state st);
void drv8871_set_speed(drv8871_t *dev, uint8_t duty);
void drv8871_sleep(drv8871_t *dev, bool sleep);

#endif // DRV8871_H
