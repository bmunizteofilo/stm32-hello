#include <assert.h>
#include <stdio.h>
#include "Motor/tb6612fng.h"

static bool ain1_state, ain2_state, bin1_state, bin2_state, stby_state;
static uint8_t pwma_duty, pwmb_duty;

static void ain1_write(bool level) { ain1_state = level; }
static void ain2_write(bool level) { ain2_state = level; }
static void bin1_write(bool level) { bin1_state = level; }
static void bin2_write(bool level) { bin2_state = level; }
static void stby_write(bool level) { stby_state = level; }
static void pwma_pwm(uint8_t duty) { pwma_duty = duty; }
static void pwmb_pwm(uint8_t duty) { pwmb_duty = duty; }

int main(void) {
    tb6612fng_t dev;
    tb6612fng_pins_t pins = {
        .ain1 = ain1_write,
        .ain2 = ain2_write,
        .pwma = pwma_pwm,
        .bin1 = bin1_write,
        .bin2 = bin2_write,
        .pwmb = pwmb_pwm,
        .stby = stby_write,
    };
    tb6612fng_init(&dev, &pins);
    assert(stby_state); /* ativo */
    assert(!ain1_state && !ain2_state && !bin1_state && !bin2_state);
    assert(pwma_duty == 0 && pwmb_duty == 0);

    tb6612fng_motor_a_set(&dev, TB6612FNG_FORWARD);
    assert(ain1_state && !ain2_state);
    tb6612fng_motor_a_set(&dev, TB6612FNG_REVERSE);
    assert(!ain1_state && ain2_state);

    tb6612fng_set_speed_a(&dev, 80);
    assert(pwma_duty == 80);

    tb6612fng_motor_b_set(&dev, TB6612FNG_BRAKE);
    assert(bin1_state && bin2_state);
    tb6612fng_set_speed_b(&dev, 150);
    assert(pwmb_duty == 150);

    /* Exemplo: giro completo para cada lado */
    tb6612fng_set_speed_a(&dev, 180);
    tb6612fng_set_speed_b(&dev, 180);
    tb6612fng_motor_a_set(&dev, TB6612FNG_FORWARD);
    tb6612fng_motor_b_set(&dev, TB6612FNG_REVERSE);
    assert(ain1_state && !ain2_state && !bin1_state && bin2_state);
    tb6612fng_motor_a_set(&dev, TB6612FNG_REVERSE);
    tb6612fng_motor_b_set(&dev, TB6612FNG_FORWARD);
    assert(!ain1_state && ain2_state && bin1_state && !bin2_state);
    tb6612fng_motor_a_set(&dev, TB6612FNG_BRAKE);
    tb6612fng_motor_b_set(&dev, TB6612FNG_BRAKE);
    assert(ain1_state && ain2_state && bin1_state && bin2_state);

    tb6612fng_standby(&dev, true); /* coloca em standby */
    assert(!stby_state);
    tb6612fng_standby(&dev, false);
    assert(stby_state);

    printf("[tb6612fng] testes OK\n");
    return 0;
}
