#ifndef MC33932_H
#define MC33932_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file mc33932.h
 * @brief Driver para o CI MC33932 (ponte H dupla).
 *
 * Exemplos de aplicação reais:
 * 1. Robôs de competição de alto torque.
 * 2. Sistemas de locomoção de cadeiras de rodas elétricas.
 * 3. Acionamento de esteiras de tanques robóticos.
 */

typedef void (*mc33932_gpio_write_t)(bool level);
typedef void (*mc33932_pwm_write_t)(uint8_t duty);

typedef struct {
    mc33932_gpio_write_t in1;
    mc33932_gpio_write_t in2;
    mc33932_pwm_write_t ena;
    mc33932_gpio_write_t in3;
    mc33932_gpio_write_t in4;
    mc33932_pwm_write_t enb;
} mc33932_pins_t;

typedef struct {
    mc33932_pins_t pins;
} mc33932_t;

enum mc33932_state {
    MC33932_COAST,
    MC33932_FORWARD,
    MC33932_REVERSE,
    MC33932_BRAKE,
};

void mc33932_init(mc33932_t *dev, const mc33932_pins_t *pins);
void mc33932_motor_a_set(mc33932_t *dev, enum mc33932_state st);
void mc33932_motor_b_set(mc33932_t *dev, enum mc33932_state st);
void mc33932_set_speed_a(mc33932_t *dev, uint8_t duty);
void mc33932_set_speed_b(mc33932_t *dev, uint8_t duty);

#endif // MC33932_H
