#ifndef BTN8982_H
#define BTN8982_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file btn8982.h
 * @brief Driver para o CI BTN8982 (ponte H de alta corrente).
 *
 * Exemplos de aplicação reais:
 * 1. Carrinhos elétricos infantis.
 * 2. Plataformas elevatórias industriais.
 * 3. Veículos off-road de controle remoto.
 */

typedef void (*btn8982_gpio_write_t)(bool level);
typedef void (*btn8982_pwm_write_t)(uint8_t duty);

typedef struct {
    btn8982_gpio_write_t in1;
    btn8982_gpio_write_t in2;
    btn8982_pwm_write_t pwm;
    btn8982_gpio_write_t enable; /* opcional */
} btn8982_pins_t;

typedef struct {
    btn8982_pins_t pins;
} btn8982_t;

enum btn8982_state {
    BTN8982_COAST,
    BTN8982_FORWARD,
    BTN8982_REVERSE,
    BTN8982_BRAKE,
};

void btn8982_init(btn8982_t *dev, const btn8982_pins_t *pins);
void btn8982_set(btn8982_t *dev, enum btn8982_state st);
void btn8982_set_speed(btn8982_t *dev, uint8_t duty);
void btn8982_enable(btn8982_t *dev, bool en);

#endif // BTN8982_H
