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
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN1 → PA0 (GPIO)
 * - IN2 → PA1 (GPIO)
 * - PWM → PA8 (TIM1_CH1 PWM)
 * - ENABLE → PB0 (GPIO, opcional)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para PWM no pino PWM.
 * 3. Se usar ENABLE, mantenha-o em alto.
 * 4. Preencha `btn8982_pins_t` com callbacks.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pb0(bool v){ gpio_write(GPIOB,0,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * void btn8982_setup(btn8982_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     btn8982_pins_t p = { pa0, pa1, pwm1, pb0 };
 *     btn8982_init(m, &p);
 * }
 * ```
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
