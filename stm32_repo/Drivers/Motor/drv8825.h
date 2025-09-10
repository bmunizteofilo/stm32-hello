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
