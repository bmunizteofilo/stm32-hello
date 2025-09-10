#ifndef TMC2209_H
#define TMC2209_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file tmc2209.h
 * @brief Driver simplificado para o controlador de passo TMC2209.
 *
 * Exemplos de aplicação reais:
 * 1. Impressoras 3D com detecção de batida (stallguard).
 * 2. Robôs de precisão com movimento silencioso.
 * 3. Sistemas de posicionamento em automação.
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
 * 2. Configure um timer para gerar pulsos em STEP ou use `tmc2209_step`.
 * 3. Ajuste MS1/MS2 para o microstepping desejado.
 *
 * ```c
 * #include "gpio.h"
 * static void dir(bool v){ gpio_write(GPIOA,0,v); }
 * static void step(void){ gpio_write(GPIOA,8,1); for(volatile int i=0;i<100;i++); gpio_write(GPIOA,8,0); }
 * static void en(bool v){ gpio_write(GPIOB,0,!v); }
 * static void ms1(bool v){ gpio_write(GPIOA,1,v); }
 * static void ms2(bool v){ gpio_write(GPIOA,2,v); }
 * void tmc2209_setup(tmc2209_t *d){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     tmc2209_pins_t p = { dir, step, en, ms1, ms2 };
 *     tmc2209_init(d, &p);
 * }
 * ```
 */

typedef void (*tmc2209_gpio_write_t)(bool level);
typedef void (*tmc2209_pulse_t)(void);

typedef struct {
    tmc2209_gpio_write_t dir;
    tmc2209_pulse_t step;
    tmc2209_gpio_write_t enable;
    tmc2209_gpio_write_t ms1;
    tmc2209_gpio_write_t ms2;
} tmc2209_pins_t;

typedef struct {
    tmc2209_pins_t pins;
} tmc2209_t;

enum tmc2209_microstep {
    TMC2209_FULL,
    TMC2209_HALF,
    TMC2209_QUARTER,
    TMC2209_EIGHTH,
    TMC2209_SIXTEENTH,
    TMC2209_THIRTYSECOND,
    TMC2209_SIXTYFOURTH,
    TMC2209_ONE_TWENTY_EIGHTH,
    TMC2209_TWO_FIFTY_SIXTH,
};

void tmc2209_init(tmc2209_t *dev, const tmc2209_pins_t *pins);
void tmc2209_step(tmc2209_t *dev);
void tmc2209_set_dir(tmc2209_t *dev, bool dir);
void tmc2209_enable(tmc2209_t *dev, bool en);
void tmc2209_set_microstep(tmc2209_t *dev, enum tmc2209_microstep ms);

#endif // TMC2209_H
