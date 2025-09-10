#ifndef MC33932_H
#define MC33932_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file mc33932.h
 * @brief Driver para o CI MC33932 (ponte H dupla).
 *
 * Exemplos de aplicação reais:
 * 1. Robôs de competição de alto torque.
 * 2. Sistemas de locomoção de cadeiras de rodas elétricas.
 * 3. Acionamento de esteiras de tanques robóticos.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - IN1 → PA0 (GPIO)
 * - IN2 → PA1 (GPIO)
 * - ENA → PA8 (TIM1_CH1 PWM)
 * - IN3 → PA2 (GPIO)
 * - IN4 → PA3 (GPIO)
 * - ENB → PA9 (TIM1_CH2 PWM)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída push-pull.
 * 2. Configure o temporizador para gerar PWM nos pinos ENA/ENB.
 * 3. Preencha `mc33932_pins_t` com callbacks para cada pino.
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
 * void mc33932_setup(mc33932_t *m){
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
 *     mc33932_pins_t p = { pa0, pa1, pwm1, pa2, pa3, pwm2 };
 *     mc33932_init(m, &p);
 * }
 * ```
 */

typedef void (*mc33932_gpio_write_t)(bool level);
typedef void (*mc33932_pwm_write_t)(uint8_t duty);

typedef struct {
    mc33932_gpio_write_t in1;
    mc33932_gpio_write_t in2;
    mc33932_pwm_write_t ena;
    mc33932_gpio_write_t in3;
    mc33932_gpio_write_t in4;
    mc33932_pwm_write_t enb;
} mc33932_pins_t;

typedef struct {
    mc33932_pins_t pins;
} mc33932_t;

enum mc33932_state {
    MC33932_COAST,
    MC33932_FORWARD,
    MC33932_REVERSE,
    MC33932_BRAKE,
};

void mc33932_init(mc33932_t *dev, const mc33932_pins_t *pins);
void mc33932_motor_a_set(mc33932_t *dev, enum mc33932_state st);
void mc33932_motor_b_set(mc33932_t *dev, enum mc33932_state st);
void mc33932_set_speed_a(mc33932_t *dev, uint8_t duty);
void mc33932_set_speed_b(mc33932_t *dev, uint8_t duty);

#endif // MC33932_H
