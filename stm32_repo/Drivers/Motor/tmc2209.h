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
