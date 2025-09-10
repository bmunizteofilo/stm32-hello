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
