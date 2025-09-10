#ifndef TB6612FNG_H
#define TB6612FNG_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file tb6612fng.h
 * @brief Driver para o CI TB6612FNG (ponte H dupla com standby).
 *
 * Exemplos de aplicação reais:
 * 1. Robô seguidor de linha com duas rodas motrizes independentes.
 *    ```c
 *    tb6612fng_t m; tb6612fng_pins_t p = {...};
 *    tb6612fng_init(&m, &p);
 *    tb6612fng_set_speed_a(&m, 200);
 *    tb6612fng_set_speed_b(&m, 200);
 *    // vira 360° para a esquerda
 *    tb6612fng_motor_a_set(&m, TB6612FNG_FORWARD);
 *    tb6612fng_motor_b_set(&m, TB6612FNG_REVERSE);
 *    // ... tempo suficiente ...
 *    // vira 360° para a direita
 *    tb6612fng_motor_a_set(&m, TB6612FNG_REVERSE);
 *    tb6612fng_motor_b_set(&m, TB6612FNG_FORWARD);
 *    tb6612fng_motor_a_set(&m, TB6612FNG_BRAKE);
 *    tb6612fng_motor_b_set(&m, TB6612FNG_BRAKE);
 *    ```
 * 2. Pequena esteira transportadora com modo standby.
 *    ```c
 *    tb6612fng_t belt; tb6612fng_pins_t pb = {...};
 *    tb6612fng_init(&belt, &pb);
 *    tb6612fng_motor_a_set(&belt, TB6612FNG_FORWARD);
 *    tb6612fng_set_speed_a(&belt, 150);
 *    // pausa economizando energia
 *    tb6612fng_standby(&belt, true);
 *    // retoma operação
 *    tb6612fng_standby(&belt, false);
 *    ```
 * 3. Carrinho de controle remoto com marcha à ré.
 *    ```c
 *    tb6612fng_t car; tb6612fng_pins_t pc = {...};
 *    tb6612fng_init(&car, &pc);
 *    tb6612fng_set_speed_a(&car, 255);
 *    tb6612fng_motor_a_set(&car, TB6612FNG_FORWARD); // acelera
 *    // freia e reverte
 *    tb6612fng_motor_a_set(&car, TB6612FNG_BRAKE);
 *    tb6612fng_motor_a_set(&car, TB6612FNG_REVERSE);
 *    ```
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - AIN1 → PA0 (GPIO)
 * - AIN2 → PA1 (GPIO)
 * - PWMA → PA8 (TIM1_CH1 PWM)
 * - BIN1 → PA2 (GPIO)
 * - BIN2 → PA3 (GPIO)
 * - PWMB → PA9 (TIM1_CH2 PWM)
 * - STBY → PB0 (GPIO para standby)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM nos pinos PWMA/PWMB.
 * 3. Deixe STBY em nível alto para habilitar o CI.
 * 4. Preencha `tb6612fng_pins_t` com callbacks adequados.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pa2(bool v){ gpio_write(GPIOA,2,v); }
 * static void pa3(bool v){ gpio_write(GPIOA,3,v); }
 * static void pb0(bool v){ gpio_write(GPIOB,0,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * static void pwm2(uint8_t d){ tim_set_compare(TIM1,2,d); }
 * void tb6612fng_setup(tb6612fng_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,3,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     gpio_config(GPIOA,9,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA9_AF2_TIM1_CH2 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_config_output(TIM1,2,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     tb6612fng_pins_t p = { pa0, pa1, pwm1, pa2, pa3, pwm2, pb0 };
 *     tb6612fng_init(m, &p);
 * }
 * ```
 */

/** Função para escrever nível lógico em um pino. */
typedef void (*tb6612fng_gpio_write_t)(bool level);
/** Função para gerar PWM no pino de controle (0-255). */
typedef void (*tb6612fng_pwm_write_t)(uint8_t duty);

typedef struct {
    tb6612fng_gpio_write_t ain1;
    tb6612fng_gpio_write_t ain2;
    tb6612fng_pwm_write_t pwma;
    tb6612fng_gpio_write_t bin1;
    tb6612fng_gpio_write_t bin2;
    tb6612fng_pwm_write_t pwmb;
    tb6612fng_gpio_write_t stby;
} tb6612fng_pins_t;

typedef struct {
    tb6612fng_pins_t pins;
} tb6612fng_t;

enum tb6612fng_motor_state {
    TB6612FNG_COAST,
    TB6612FNG_FORWARD,
    TB6612FNG_REVERSE,
    TB6612FNG_BRAKE,
};

void tb6612fng_init(tb6612fng_t *dev, const tb6612fng_pins_t *pins);
void tb6612fng_motor_a_set(tb6612fng_t *dev, enum tb6612fng_motor_state state);
void tb6612fng_motor_b_set(tb6612fng_t *dev, enum tb6612fng_motor_state state);
void tb6612fng_set_speed_a(tb6612fng_t *dev, uint8_t duty);
void tb6612fng_set_speed_b(tb6612fng_t *dev, uint8_t duty);
void tb6612fng_standby(tb6612fng_t *dev, bool standby);

#endif // TB6612FNG_H
