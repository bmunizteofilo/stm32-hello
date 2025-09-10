#ifndef DRV8825_H
#define DRV8825_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8825.h
 * @brief Driver para o controlador de motor de passo DRV8825.
 *
 * Exemplos de aplicação reais:
 * 1. Impressoras 3D de alto desempenho.
 * 2. Plotters de desenho.
 * 3. Controle de máquinas pick-and-place.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - DIR → PA0 (GPIO)
 * - STEP → PA8 (TIM1_CH1 ou pulso gerado por software)
 * - ENABLE → PB0 (GPIO, nível baixo ativa)
 * - MS1 → PA1 (GPIO)
 * - MS2 → PA2 (GPIO)
 * - MS3 → PA3 (GPIO)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída.
 * 2. Configure um timer para gerar pulsos periódicos no pino STEP ou use `drv8825_step`.
 * 3. Ajuste os pinos MS1‑MS3 conforme o microstepping desejado.
 *
 * ```c
 * #include "gpio.h"
 * static void dir(bool v){ gpio_write(GPIOA,0,v); }
 * static void step(void){ gpio_write(GPIOA,8,1); for(volatile int i=0;i<100;i++); gpio_write(GPIOA,8,0); }
 * static void en(bool v){ gpio_write(GPIOB,0,!v); }
 * static void ms1(bool v){ gpio_write(GPIOA,1,v); }
 * static void ms2(bool v){ gpio_write(GPIOA,2,v); }
 * static void ms3(bool v){ gpio_write(GPIOA,3,v); }
 * void drv8825_setup(drv8825_t *d){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,3,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     drv8825_pins_t p = { dir, step, en, ms1, ms2, ms3 };
 *     drv8825_init(d, &p);
 * }
 * ```
 */

typedef void (*drv8825_gpio_write_t)(bool level);
typedef void (*drv8825_pulse_t)(void);

typedef struct {
    drv8825_gpio_write_t dir;
    drv8825_pulse_t step;
    drv8825_gpio_write_t enable;
    drv8825_gpio_write_t ms1;
    drv8825_gpio_write_t ms2;
    drv8825_gpio_write_t ms3;
} drv8825_pins_t;

typedef struct {
    drv8825_pins_t pins;
} drv8825_t;

enum drv8825_microstep {
    DRV8825_FULL,
    DRV8825_HALF,
    DRV8825_QUARTER,
    DRV8825_EIGHTH,
    DRV8825_SIXTEENTH,
    DRV8825_THIRTYSECOND,
};

void drv8825_init(drv8825_t *dev, const drv8825_pins_t *pins);
void drv8825_step(drv8825_t *dev);
void drv8825_set_dir(drv8825_t *dev, bool dir);
void drv8825_enable(drv8825_t *dev, bool en);
void drv8825_set_microstep(drv8825_t *dev, enum drv8825_microstep ms);

#endif // DRV8825_H
