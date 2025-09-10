#ifndef DRV8833_H
#define DRV8833_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8833.h
 * @brief Driver para o CI DRV8833 (ponte H dupla de baixa tensão).
 *
 * Exemplos de aplicação reais:
 * 1. Pequeno robô seguidor de linha com duas rodas independentes.
 *    ```c
 *    drv8833_t m; drv8833_pins_t p = {...};
 *    drv8833_init(&m, &p);
 *    drv8833_set_speed_a(&m, 200);
 *    drv8833_set_speed_b(&m, 200);
 *    drv8833_motor_a_set(&m, DRV8833_FORWARD);
 *    drv8833_motor_b_set(&m, DRV8833_FORWARD);
 *    ```
 * 2. Controle de esteira transportadora compacta.
 *    ```c
 *    drv8833_t conv; drv8833_pins_t pc = {...};
 *    drv8833_init(&conv, &pc);
 *    drv8833_set_speed_a(&conv, 128);
 *    drv8833_motor_a_set(&conv, DRV8833_FORWARD);
 *    ```
 * 3. Sistema de abertura de cortinas automáticas.
 *    ```c
 *    drv8833_t curtain; drv8833_pins_t pp = {...};
 *    drv8833_init(&curtain, &pp);
 *    drv8833_motor_a_set(&curtain, DRV8833_FORWARD); // abre
 *    drv8833_motor_a_set(&curtain, DRV8833_REVERSE); // fecha
 *    ```
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - AIN1 → PA0 (GPIO)
 * - AIN2 → PA1 (GPIO)
 * - PWMA → PA8 (TIM1_CH1 PWM)
 * - BIN1 → PA2 (GPIO)
 * - BIN2 → PA3 (GPIO)
 * - PWMB → PA9 (TIM1_CH2 PWM)
 * - SLEEP → PB0 (GPIO, manter alto para ativo)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para PWM nos pinos PWMA/PWMB.
 * 3. Inicialmente coloque SLEEP em alto.
 * 4. Preencha `drv8833_pins_t` com callbacks para cada pino.
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
 * void drv8833_setup(drv8833_t *m){
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
 *     drv8833_pins_t p = { pa0, pa1, pwm1, pa2, pa3, pwm2, pb0 };
 *     drv8833_init(m, &p);
 * }
 * ```
 */

typedef void (*drv8833_gpio_write_t)(bool level);
typedef void (*drv8833_pwm_write_t)(uint8_t duty);

typedef struct {
    drv8833_gpio_write_t ain1;
    drv8833_gpio_write_t ain2;
    drv8833_pwm_write_t pwma;
    drv8833_gpio_write_t bin1;
    drv8833_gpio_write_t bin2;
    drv8833_pwm_write_t pwmb;
    drv8833_gpio_write_t sleep; /* opcional */
} drv8833_pins_t;

typedef struct {
    drv8833_pins_t pins;
} drv8833_t;

enum drv8833_state {
    DRV8833_COAST,
    DRV8833_FORWARD,
    DRV8833_REVERSE,
    DRV8833_BRAKE,
};

void drv8833_init(drv8833_t *dev, const drv8833_pins_t *pins);
void drv8833_motor_a_set(drv8833_t *dev, enum drv8833_state state);
void drv8833_motor_b_set(drv8833_t *dev, enum drv8833_state state);
void drv8833_set_speed_a(drv8833_t *dev, uint8_t duty);
void drv8833_set_speed_b(drv8833_t *dev, uint8_t duty);
void drv8833_sleep(drv8833_t *dev, bool sleep);

#endif // DRV8833_H
