#ifndef VNH2SP30_H
#define VNH2SP30_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file vnh2sp30.h
 * @brief Driver para o CI VNH2SP30 (ponte H de 30 A).
 *
 * Exemplos de aplicação reais:
 * 1. Acionamento de motores de robores de combate.
 * 2. Plataformas robóticas de grande porte.
 * 3. Veículos utilitários elétricos leves.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN_A → PA0 (GPIO)
 * - IN_B → PA1 (GPIO)
 * - PWM → PA8 (TIM1_CH1 PWM)
 * - ENABLE → PB0 (GPIO)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM no pino PWM.
 * 3. Mantenha ENABLE em alto e preencha `vnh2sp30_pins_t` com callbacks.
 *
 * ```c
 * #include "gpio.h"
 * #include "tim.h"
 * #include "gpio_af.h"
 * static void pa0(bool v){ gpio_write(GPIOA,0,v); }
 * static void pa1(bool v){ gpio_write(GPIOA,1,v); }
 * static void pb0(bool v){ gpio_write(GPIOB,0,v); }
 * static void pwm1(uint8_t d){ tim_set_compare(TIM1,1,d); }
 * void vnh2sp30_setup(vnh2sp30_t *m){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_AF, .alternate=PA8_AF2_TIM1_CH1 });
 *     tim_init(TIM1,&(tim_cfg_t){ .mode=TIM_COUNTER_UP, .prescaler=47, .period=255 });
 *     tim_config_output(TIM1,1,&(tim_oc_cfg_t){ .mode=TIM_OC_PWM1 });
 *     tim_enable(TIM1,true);
 *     vnh2sp30_pins_t p = { pa0, pa1, pwm1, pb0 };
 *     vnh2sp30_init(m, &p);
 * }
 * ```
 */

typedef void (*vnh2sp30_gpio_write_t)(bool level);
typedef void (*vnh2sp30_pwm_write_t)(uint8_t duty);

typedef struct {
    vnh2sp30_gpio_write_t in_a;
    vnh2sp30_gpio_write_t in_b;
    vnh2sp30_pwm_write_t pwm;
    vnh2sp30_gpio_write_t enable;
} vnh2sp30_pins_t;

typedef struct {
    vnh2sp30_pins_t pins;
} vnh2sp30_t;

enum vnh2sp30_state {
    VNH2SP30_COAST,
    VNH2SP30_FORWARD,
    VNH2SP30_REVERSE,
    VNH2SP30_BRAKE,
};

void vnh2sp30_init(vnh2sp30_t *dev, const vnh2sp30_pins_t *pins);
void vnh2sp30_set(vnh2sp30_t *dev, enum vnh2sp30_state st);
void vnh2sp30_set_speed(vnh2sp30_t *dev, uint8_t duty);
void vnh2sp30_enable(vnh2sp30_t *dev, bool en);

#endif // VNH2SP30_H
