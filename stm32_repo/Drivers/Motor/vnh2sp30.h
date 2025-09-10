#ifndef VNH2SP30_H
#define VNH2SP30_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file vnh2sp30.h
 * @brief Driver para o CI VNH2SP30 (ponte H de 30 A).
 *
 * Exemplos de aplicação reais:
 * 1. Acionamento de motores de robores de combate.
 * 2. Plataformas robóticas de grande porte.
 * 3. Veículos utilitários elétricos leves.
 */

typedef void (*vnh2sp30_gpio_write_t)(bool level);
typedef void (*vnh2sp30_pwm_write_t)(uint8_t duty);

typedef struct {
    vnh2sp30_gpio_write_t in_a;
    vnh2sp30_gpio_write_t in_b;
    vnh2sp30_pwm_write_t pwm;
    vnh2sp30_gpio_write_t enable;
} vnh2sp30_pins_t;

typedef struct {
    vnh2sp30_pins_t pins;
} vnh2sp30_t;

enum vnh2sp30_state {
    VNH2SP30_COAST,
    VNH2SP30_FORWARD,
    VNH2SP30_REVERSE,
    VNH2SP30_BRAKE,
};

void vnh2sp30_init(vnh2sp30_t *dev, const vnh2sp30_pins_t *pins);
void vnh2sp30_set(vnh2sp30_t *dev, enum vnh2sp30_state st);
void vnh2sp30_set_speed(vnh2sp30_t *dev, uint8_t duty);
void vnh2sp30_enable(vnh2sp30_t *dev, bool en);

#endif // VNH2SP30_H
