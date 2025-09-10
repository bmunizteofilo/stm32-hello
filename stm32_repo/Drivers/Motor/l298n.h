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
 * 2. Configure o temporizador para gerar PWM de 0‑255 nos pinos ENA/ENB.
 * 3. Preencha `l298n_pins_t` com callbacks que acionem esses pinos.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pa2(bool v){ gpio_write(GPIOA,2,v); }
 * static void pa3(bool v){ gpio_write(GPIOA,3,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * static void pwm2(uint8_t d){ tim_set_compare(TIM1,2,d); }
 * void l298n_setup(l298n_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,3,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     gpio_config(GPIOA,9,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA9_AF2_TIM1_CH2 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_config_output(TIM1,2,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     l298n_pins_t p = { pa0, pa1, pwm1, pa2, pa3, pwm2 };
 *     l298n_init(m, &p);
 * }
 * ```
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
