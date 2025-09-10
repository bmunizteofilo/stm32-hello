#ifndef BTS7960_H
#define BTS7960_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file bts7960.h
 * @brief Driver para o módulo BTS7960 (ponte H de 43 A).
 *
 * Exemplos de aplicação reais:
 * 1. Conversão de carrinhos de golfe para elétricos.
 * 2. Guindastes e guinchos de pequeno porte.
 * 3. Robôs sumô de alta potência.
 */

typedef void (*bts7960_gpio_write_t)(bool level);
typedef void (*bts7960_pwm_write_t)(uint8_t duty);

typedef struct {
    bts7960_pwm_write_t rpwm;
    bts7960_pwm_write_t lpwm;
    bts7960_gpio_write_t ren;
    bts7960_gpio_write_t len;
} bts7960_pins_t;

enum bts7960_state {
    BTS7960_COAST,
    BTS7960_FORWARD,
    BTS7960_REVERSE,
    BTS7960_BRAKE,
};

typedef struct {
    bts7960_pins_t pins;
    enum bts7960_state state;
    uint8_t speed;
} bts7960_t;

void bts7960_init(bts7960_t *dev, const bts7960_pins_t *pins);
void bts7960_set(bts7960_t *dev, enum bts7960_state st);
void bts7960_set_speed(bts7960_t *dev, uint8_t duty);
void bts7960_enable(bts7960_t *dev, bool en);

#endif // BTS7960_H
