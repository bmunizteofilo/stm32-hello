#ifndef A4988_H
#define A4988_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file a4988.h
 * @brief Driver para o controlador de motor de passo A4988.
 *
 * Exemplos de aplicação reais:
 * 1. Impressoras 3D de mesa.
 * 2. CNCs caseiras de pequeno porte.
 * 3. Controle de sliders de câmeras para filmagem.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - DIR → PA0 (GPIO)
 * - STEP → PA8 (TIM1_CH1 ou pulso por software)
 * - ENABLE → PB0 (GPIO, nível baixo ativa)
 * - MS1 → PA1 (GPIO)
 * - MS2 → PA2 (GPIO)
 * - MS3 → PA3 (GPIO)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída.
 * 2. Configure um timer para gerar pulsos no pino STEP ou use `a4988_step`.
 * 3. Ajuste MS1‑MS3 conforme o microstepping desejado.
 *
 * ```c
 * #include "gpio.h"
 * static void dir(bool v){ gpio_write(GPIOA,0,v); }
 * static void step(void){ gpio_write(GPIOA,8,1); for(volatile int i=0;i<100;i++); gpio_write(GPIOA,8,0); }
 * static void en(bool v){ gpio_write(GPIOB,0,!v); }
 * static void ms1(bool v){ gpio_write(GPIOA,1,v); }
 * static void ms2(bool v){ gpio_write(GPIOA,2,v); }
 * static void ms3(bool v){ gpio_write(GPIOA,3,v); }
 * void a4988_setup(a4988_t *d){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,3,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     a4988_pins_t p = { dir, step, en, ms1, ms2, ms3 };
 *     a4988_init(d, &p);
 * }
 * ```
 */

typedef void (*a4988_gpio_write_t)(bool level);
typedef void (*a4988_pulse_t)(void);

typedef struct {
    a4988_gpio_write_t dir;
    a4988_pulse_t step;
    a4988_gpio_write_t enable;
    a4988_gpio_write_t ms1;
    a4988_gpio_write_t ms2;
    a4988_gpio_write_t ms3;
} a4988_pins_t;

typedef struct {
    a4988_pins_t pins;
} a4988_t;

enum a4988_microstep { A4988_FULL, A4988_HALF, A4988_QUARTER, A4988_EIGHTH, A4988_SIXTEENTH };

void a4988_init(a4988_t *dev, const a4988_pins_t *pins);
void a4988_step(a4988_t *dev);
void a4988_set_dir(a4988_t *dev, bool dir);
void a4988_enable(a4988_t *dev, bool en);
void a4988_set_microstep(a4988_t *dev, enum a4988_microstep ms);

#endif // A4988_H
