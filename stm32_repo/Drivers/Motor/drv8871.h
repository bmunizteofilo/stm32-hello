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
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN1 → PA0 (GPIO)
 * - IN2 → PA1 (GPIO)
 * - PWM → PA8 (TIM1_CH1 PWM ligado ao IN1)
 * - SLEEP → PB0 (GPIO, manter alto para ativo)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM em IN1.
 * 3. Coloque SLEEP em nível alto e preencha `drv8871_pins_t` com callbacks.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pb0(bool v){ gpio_write(GPIOB,0,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * void drv8871_setup(drv8871_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     drv8871_pins_t p = { pa0, pa1, pwm1, pb0 };
 *     drv8871_init(m, &p);
 * }
 * ```
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
