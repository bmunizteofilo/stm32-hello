#ifndef BTS7960_H
#define BTS7960_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file bts7960.h
 * @brief Driver para o módulo BTS7960 (ponte H de 43 A).
 *
 * Exemplos de aplicação reais:
 * 1. Conversão de carrinhos de golfe para elétricos.
 * 2. Guindastes e guinchos de pequeno porte.
 * 3. Robôs sumô de alta potência.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - RPWM → PA8 (TIM1_CH1 PWM)
 * - LPWM → PA9 (TIM1_CH2 PWM)
 * - REN → PA0 (GPIO)
 * - LEN → PA1 (GPIO)
 *
 * ## Inicialização
 * 1. Configure REN/LEN como GPIO de saída e mantenha-os altos para habilitar.
 * 2. Configure o temporizador para gerar PWM em RPWM/LPWM.
 * 3. Preencha `bts7960_pins_t` com callbacks para os pinos.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * static void pwm2(uint8_t d){ tim_set_compare(TIM1,2,d); }
 * void bts7960_setup(bts7960_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     gpio_config(GPIOA,9,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA9_AF2_TIM1_CH2 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_config_output(TIM1,2,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     bts7960_pins_t p = { pwm1, pwm2, pa0, pa1 };
 *     bts7960_init(m, &p);
 * }
 * ```
 */

typedef void (*bts7960_gpio_write_t)(bool level);
typedef void (*bts7960_pwm_write_t)(uint8_t duty);

typedef struct {
    bts7960_pwm_write_t rpwm;
    bts7960_pwm_write_t lpwm;
    bts7960_gpio_write_t ren;
    bts7960_gpio_write_t len;
} bts7960_pins_t;

enum bts7960_state {
    BTS7960_COAST,
    BTS7960_FORWARD,
    BTS7960_REVERSE,
    BTS7960_BRAKE,
};

typedef struct {
    bts7960_pins_t pins;
    enum bts7960_state state;
    uint8_t speed;
} bts7960_t;

void bts7960_init(bts7960_t *dev, const bts7960_pins_t *pins);
void bts7960_set(bts7960_t *dev, enum bts7960_state st);
void bts7960_set_speed(bts7960_t *dev, uint8_t duty);
void bts7960_enable(bts7960_t *dev, bool en);

#endif // BTS7960_H
