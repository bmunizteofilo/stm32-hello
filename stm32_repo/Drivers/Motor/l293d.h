#ifndef L293D_H
#define L293D_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file l293d.h
 * @brief Driver para o CI L293D (ponte H dupla com diodos internos).
 *
 * Exemplos de aplicação reais:
 * 1. Mini robô diferencial que gira 360° para calibrar sensores.
 *    ```c
 *    l293d_t m; l293d_pins_t p = {...};
 *    l293d_init(&m, &p);
 *    l293d_set_speed_a(&m, 220);
 *    l293d_set_speed_b(&m, 220);
 *    l293d_motor_a_set(&m, L293D_FORWARD);
 *    l293d_motor_b_set(&m, L293D_REVERSE);
 *    // ... tempo para completar a volta ...
 *    l293d_motor_a_set(&m, L293D_REVERSE);
 *    l293d_motor_b_set(&m, L293D_FORWARD);
 *    ```
 * 2. Controle de duas ventoinhas independentes com PWM.
 *    ```c
 *    l293d_t fan; l293d_pins_t pf = {...};
 *    l293d_init(&fan, &pf);
 *    l293d_set_speed_a(&fan, 128); // meia velocidade
 *    l293d_motor_a_set(&fan, L293D_FORWARD);
 *    l293d_set_speed_b(&fan, 200);
 *    l293d_motor_b_set(&fan, L293D_FORWARD);
 *    ```
 * 3. Acionamento de um pequeno portão deslizante (atuador).
 *    ```c
 *    l293d_t gate; l293d_pins_t pg = {...};
 *    l293d_init(&gate, &pg);
 *    l293d_motor_a_set(&gate, L293D_FORWARD);  // abre
 *    // ... sensor de fim de curso ...
 *    l293d_motor_a_set(&gate, L293D_REVERSE);  // fecha
 *    ```
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN1 → PA0 (GPIO saída)
 * - IN2 → PA1 (GPIO saída)
 * - ENA → PA8 (TIM1_CH1 PWM)
 * - IN3 → PA2 (GPIO saída)
 * - IN4 → PA3 (GPIO saída)
 * - ENB → PA9 (TIM1_CH2 PWM)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM nos pinos ENA/ENB.
 * 3. Preencha `l293d_pins_t` com callbacks correspondentes.
 */

/** Função para escrever nível lógico em um pino. */
typedef void (*l293d_gpio_write_t)(bool level);
/** Função para gerar PWM no pino de enable (0-255). */
typedef void (*l293d_pwm_write_t)(uint8_t duty);

typedef struct {
    l293d_gpio_write_t in1;
    l293d_gpio_write_t in2;
    l293d_pwm_write_t ena;
    l293d_gpio_write_t in3;
    l293d_gpio_write_t in4;
    l293d_pwm_write_t enb;
} l293d_pins_t;

typedef struct {
    l293d_pins_t pins;
} l293d_t;

enum l293d_motor_state {
    L293D_COAST,
    L293D_FORWARD,
    L293D_REVERSE,
    L293D_BRAKE,
};

void l293d_init(l293d_t *dev, const l293d_pins_t *pins);
void l293d_motor_a_set(l293d_t *dev, enum l293d_motor_state state);
void l293d_motor_b_set(l293d_t *dev, enum l293d_motor_state state);
void l293d_set_speed_a(l293d_t *dev, uint8_t duty);
void l293d_set_speed_b(l293d_t *dev, uint8_t duty);

#endif // L293D_H
