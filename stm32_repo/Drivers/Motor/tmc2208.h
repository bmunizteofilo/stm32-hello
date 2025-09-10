#ifndef TMC2208_H
#define TMC2208_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file tmc2208.h
 * @brief Driver simplificado para o controlador de passo TMC2208.
 *
 * Exemplos de aplicação reais:
 * 1. Impressoras 3D silenciosas com tecnologia stealthChop.
 * 2. Sistemas de foco motorizado em câmeras.
 * 3. Automatização residencial com mecanismos discretos.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - DIR → PA0 (GPIO)
 * - STEP → PA8 (TIM1_CH1 ou pulso por software)
 * - ENABLE → PB0 (GPIO, nível baixo ativa)
 * - MS1 → PA1 (GPIO)
 * - MS2 → PA2 (GPIO)
 *
 * ## Inicialização
 * 1. Configure os GPIOs como saída.
 * 2. Configure um timer para gerar pulsos em STEP ou use `tmc2208_step`.
 * 3. Ajuste MS1/MS2 conforme o microstepping desejado.
 *
 * ```c
 * #include "gpio.h"
 * static void dir(bool v){ gpio_write(GPIOA,0,v); }
 * static void step(void){ gpio_write(GPIOA,8,1); for(volatile int i=0;i<100;i++); gpio_write(GPIOA,8,0); }
 * static void en(bool v){ gpio_write(GPIOB,0,!v); }
 * static void ms1(bool v){ gpio_write(GPIOA,1,v); }
 * static void ms2(bool v){ gpio_write(GPIOA,2,v); }
 * void tmc2208_setup(tmc2208_t *d){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     tmc2208_pins_t p = { dir, step, en, ms1, ms2 };
 *     tmc2208_init(d, &p);
 * }
 * ```
 */

typedef void (*tmc2208_gpio_write_t)(bool level);
typedef void (*tmc2208_pulse_t)(void);

typedef struct {
    tmc2208_gpio_write_t dir;
    tmc2208_pulse_t step;
    tmc2208_gpio_write_t enable;
    tmc2208_gpio_write_t ms1;
    tmc2208_gpio_write_t ms2;
} tmc2208_pins_t;

typedef struct {
    tmc2208_pins_t pins;
} tmc2208_t;

enum tmc2208_microstep {
    TMC2208_FULL,
    TMC2208_HALF,
    TMC2208_QUARTER,
    TMC2208_EIGHTH,
    TMC2208_SIXTEENTH,
    TMC2208_THIRTYSECOND,
    TMC2208_SIXTYFOURTH,
    TMC2208_ONE_TWENTY_EIGHTH,
    TMC2208_TWO_FIFTY_SIXTH,
};

void tmc2208_init(tmc2208_t *dev, const tmc2208_pins_t *pins);
void tmc2208_step(tmc2208_t *dev);
void tmc2208_set_dir(tmc2208_t *dev, bool dir);
void tmc2208_enable(tmc2208_t *dev, bool en);
void tmc2208_set_microstep(tmc2208_t *dev, enum tmc2208_microstep ms);

#endif // TMC2208_H
