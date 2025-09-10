#ifndef DRV8833_H
#define DRV8833_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file drv8833.h
 * @brief Driver para o CI DRV8833 (ponte H dupla de baixa tensão).
 *
 * Exemplos de aplicação reais:
 * 1. Pequeno robô seguidor de linha com duas rodas independentes.
 *    ```c
 *    drv8833_t m; drv8833_pins_t p = {...};
 *    drv8833_init(&m, &p);
 *    drv8833_set_speed_a(&m, 200);
 *    drv8833_set_speed_b(&m, 200);
 *    drv8833_motor_a_set(&m, DRV8833_FORWARD);
 *    drv8833_motor_b_set(&m, DRV8833_FORWARD);
 *    ```
 * 2. Controle de esteira transportadora compacta.
 *    ```c
 *    drv8833_t conv; drv8833_pins_t pc = {...};
 *    drv8833_init(&conv, &pc);
 *    drv8833_set_speed_a(&conv, 128);
 *    drv8833_motor_a_set(&conv, DRV8833_FORWARD);
 *    ```
 * 3. Sistema de abertura de cortinas automáticas.
 *    ```c
 *    drv8833_t curtain; drv8833_pins_t pp = {...};
 *    drv8833_init(&curtain, &pp);
 *    drv8833_motor_a_set(&curtain, DRV8833_FORWARD); // abre
 *    drv8833_motor_a_set(&curtain, DRV8833_REVERSE); // fecha
 *    ```
 */

typedef void (*drv8833_gpio_write_t)(bool level);
typedef void (*drv8833_pwm_write_t)(uint8_t duty);

typedef struct {
    drv8833_gpio_write_t ain1;
    drv8833_gpio_write_t ain2;
    drv8833_pwm_write_t pwma;
    drv8833_gpio_write_t bin1;
    drv8833_gpio_write_t bin2;
    drv8833_pwm_write_t pwmb;
    drv8833_gpio_write_t sleep; /* opcional */
} drv8833_pins_t;

typedef struct {
    drv8833_pins_t pins;
} drv8833_t;

enum drv8833_state {
    DRV8833_COAST,
    DRV8833_FORWARD,
    DRV8833_REVERSE,
    DRV8833_BRAKE,
};

void drv8833_init(drv8833_t *dev, const drv8833_pins_t *pins);
void drv8833_motor_a_set(drv8833_t *dev, enum drv8833_state state);
void drv8833_motor_b_set(drv8833_t *dev, enum drv8833_state state);
void drv8833_set_speed_a(drv8833_t *dev, uint8_t duty);
void drv8833_set_speed_b(drv8833_t *dev, uint8_t duty);
void drv8833_sleep(drv8833_t *dev, bool sleep);

#endif // DRV8833_H
