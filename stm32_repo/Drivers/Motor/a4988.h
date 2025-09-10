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
