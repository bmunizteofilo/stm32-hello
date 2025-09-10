#ifndef TMC2130_H
#define TMC2130_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file tmc2130.h
 * @brief Driver simplificado para o controlador de passo TMC2130 com interface SPI.
 *
 * Exemplos de aplicação reais:
 * 1. Impressoras 3D com sensorless homing.
 * 2. Sistemas de posicionamento de precisão.
 * 3. Controle de eixos em robôs cartesianos.
 *
 * ## Ligações ao microcontrolador (ex. STM32)
 * - DIR → PA0 (GPIO)
 * - STEP → PA8 (TIM1_CH1 ou pulso por software)
 * - ENABLE → PB0 (GPIO, nível baixo ativa)
 * - CS → PB1 (GPIO para selecionar o driver)
 * - SCK → PB2 (GPIO para clock SPI por bit-bang)
 * - SDI → PB3 (GPIO para dados SPI)
 *
 * ## Inicialização
 * 1. Configure todos os GPIOs como saída.
 * 2. Configure um timer ou rotina para gerar pulsos em STEP.
 * 3. Implemente rotinas de bit-bang SPI usando CS/SCK/SDI se necessário.
 *
 * ```c
 * #include "gpio.h"
 * static void dir(bool v){ gpio_write(GPIOA,0,v); }
 * static void step(void){ gpio_write(GPIOA,8,1); for(volatile int i=0;i<100;i++); gpio_write(GPIOA,8,0); }
 * static void en(bool v){ gpio_write(GPIOB,0,!v); }
 * static void cs(bool v){ gpio_write(GPIOB,1,v); }
 * static void sck(bool v){ gpio_write(GPIOB,2,v); }
 * static void sdi(bool v){ gpio_write(GPIOB,3,v); }
 * void tmc2130_setup(tmc2130_t *d){
 *     gpio_config(GPIOA,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOA,8,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,0,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,1,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,2,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     gpio_config(GPIOB,3,&(gpio_cfg_t){ .mode=GPIO_MODE_OUTPUT, .otype=GPIO_OTYPE_PP });
 *     tmc2130_pins_t p = { dir, step, en, cs, sck, sdi };
 *     tmc2130_init(d, &p);
 * }
 * ```
 */

typedef void (*tmc2130_gpio_write_t)(bool level);
typedef void (*tmc2130_pulse_t)(void);

typedef struct {
    tmc2130_gpio_write_t dir;
    tmc2130_pulse_t step;
    tmc2130_gpio_write_t enable;
    tmc2130_gpio_write_t cs;
    tmc2130_gpio_write_t sck;
    tmc2130_gpio_write_t sdi;
} tmc2130_pins_t;

typedef struct {
    tmc2130_pins_t pins;
} tmc2130_t;

enum tmc2130_microstep {
    TMC2130_FULL,
    TMC2130_HALF,
    TMC2130_QUARTER,
    TMC2130_EIGHTH,
    TMC2130_SIXTEENTH,
    TMC2130_THIRTYSECOND,
    TMC2130_SIXTYFOURTH,
    TMC2130_ONE_TWENTY_EIGHTH,
    TMC2130_TWO_FIFTY_SIXTH,
};

void tmc2130_init(tmc2130_t *dev, const tmc2130_pins_t *pins);
void tmc2130_step(tmc2130_t *dev);
void tmc2130_set_dir(tmc2130_t *dev, bool dir);
void tmc2130_enable(tmc2130_t *dev, bool en);
void tmc2130_set_microstep(tmc2130_t *dev, enum tmc2130_microstep ms);

#endif // TMC2130_H
