#ifndef L298N_H
#define L298N_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file l298n.h
 * @brief Driver para o CI L298N (ponte H dupla).
 *
 * Exemplos de aplicação reais:
 * 1. Controle de duas rodas de um robô diferencial.
 *    ```c
 *    // Gira o robô uma volta completa para cada lado
 *    l298n_t m; l298n_pins_t p = {...};
 *    l298n_init(&m, &p);
 *    l298n_set_speed_a(&m, 255);
 *    l298n_set_speed_b(&m, 255);
 *    // Sentido horário
 *    l298n_motor_a_set(&m, L298N_FORWARD);
 *    l298n_motor_b_set(&m, L298N_REVERSE);
 *    // ... esperar 360° ...
 *    // Sentido anti-horário
 *    l298n_motor_a_set(&m, L298N_REVERSE);
 *    l298n_motor_b_set(&m, L298N_FORWARD);
 *    // Freia
 *    l298n_motor_a_set(&m, L298N_BRAKE);
 *    l298n_motor_b_set(&m, L298N_BRAKE);
 *    ```
 * 2. Acionamento de duas ventoinhas DC com controle de velocidade.
 *    ```c
 *    l298n_t fan; l298n_pins_t pf = {...};
 *    l298n_init(&fan, &pf);
 *    l298n_set_speed_a(&fan, 128); // meia velocidade
 *    l298n_motor_a_set(&fan, L298N_FORWARD);
 *    ```
 * 3. Operação de um atuador linear com movimento de ida e volta.
 *    ```c
 *    l298n_t act; l298n_pins_t pa = {...};
 *    l298n_init(&act, &pa);
 *    l298n_motor_a_set(&act, L298N_FORWARD);  // estende
 *    // ... aguarda fim de curso ...
 *    l298n_motor_a_set(&act, L298N_REVERSE);  // recolhe
 *    ```
 */

/** Função para escrever nível lógico em um pino. */
typedef void (*l298n_gpio_write_t)(bool level);
/** Função para gerar PWM no pino de enable (0-255). */
typedef void (*l298n_pwm_write_t)(uint8_t duty);

typedef struct {
    l298n_gpio_write_t in1;
    l298n_gpio_write_t in2;
    l298n_pwm_write_t ena;
    l298n_gpio_write_t in3;
    l298n_gpio_write_t in4;
    l298n_pwm_write_t enb;
} l298n_pins_t;

typedef struct {
    l298n_pins_t pins;
} l298n_t;

enum l298n_motor_state {
    L298N_COAST,
    L298N_FORWARD,
    L298N_REVERSE,
    L298N_BRAKE,
};

void l298n_init(l298n_t *dev, const l298n_pins_t *pins);
void l298n_motor_a_set(l298n_t *dev, enum l298n_motor_state state);
void l298n_motor_b_set(l298n_t *dev, enum l298n_motor_state state);
void l298n_set_speed_a(l298n_t *dev, uint8_t duty);
void l298n_set_speed_b(l298n_t *dev, uint8_t duty);

#endif // L298N_H
