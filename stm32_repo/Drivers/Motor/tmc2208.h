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
