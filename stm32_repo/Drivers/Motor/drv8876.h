#ifndef DRV8876_H
#define DRV8876_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8876.h
 * @brief Driver para o CI DRV8876 (ponte H simples com controle de corrente).
 *
 * Exemplos de aplicação reais:
 * 1. Atuadores de freio eletromecânico.
 * 2. Válvulas de controle proporcional.
 * 3. Veículos de brinquedo com reversão.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN1 → PA0 (GPIO)
 * - IN2 → PA1 (GPIO)
 * - PWM → PA8 (TIM1_CH1 PWM)
 * - SLEEP → PB0 (GPIO, manter alto para ativo)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM no pino PWM.
 * 3. Coloque SLEEP em alto e preencha `drv8876_pins_t` com callbacks.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pb0(bool v){ gpio_write(GPIOB,0,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * void drv8876_setup(drv8876_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     drv8876_pins_t p = { pa0, pa1, pwm1, pb0 };
 *     drv8876_init(m, &p);
 * }
 * ```
 */

typedef void (*drv8876_gpio_write_t)(bool level);
typedef void (*drv8876_pwm_write_t)(uint8_t duty);

typedef struct {
    drv8876_gpio_write_t in1;
    drv8876_gpio_write_t in2;
    drv8876_pwm_write_t pwm;
    drv8876_gpio_write_t sleep; /* opcional */
} drv8876_pins_t;

typedef struct {
    drv8876_pins_t pins;
} drv8876_t;

enum drv8876_state {
    DRV8876_COAST,
    DRV8876_FORWARD,
    DRV8876_REVERSE,
    DRV8876_BRAKE,
};

void drv8876_init(drv8876_t *dev, const drv8876_pins_t *pins);
void drv8876_set(drv8876_t *dev, enum drv8876_state st);
void drv8876_set_speed(drv8876_t *dev, uint8_t duty);
void drv8876_sleep(drv8876_t *dev, bool sleep);

#endif // DRV8876_H
