#include <assert.h>
#include <stdio.h>
#include "Motor/l298n.h"

static bool in1_state, in2_state, in3_state, in4_state;
static uint8_t ena_duty, enb_duty;

static void in1_write(bool level) { in1_state = level; }
static void in2_write(bool level) { in2_state = level; }
static void in3_write(bool level) { in3_state = level; }
static void in4_write(bool level) { in4_state = level; }
static void ena_pwm(uint8_t duty) { ena_duty = duty; }
static void enb_pwm(uint8_t duty) { enb_duty = duty; }

int main(void) {
    l298n_t dev;
    l298n_pins_t pins = {
        .in1 = in1_write,
        .in2 = in2_write,
        .ena = ena_pwm,
        .in3 = in3_write,
        .in4 = in4_write,
        .enb = enb_pwm,
    };
    l298n_init(&dev, &pins);
    assert(!in1_state && !in2_state && !in3_state && !in4_state);
    assert(ena_duty == 0 && enb_duty == 0);

    l298n_motor_a_set(&dev, L298N_FORWARD);
    assert(in1_state && !in2_state);
    l298n_motor_a_set(&dev, L298N_REVERSE);
    assert(!in1_state && in2_state);

    l298n_set_speed_a(&dev, 123);
    assert(ena_duty == 123);

    l298n_motor_b_set(&dev, L298N_BRAKE);
    assert(in3_state && in4_state);
    l298n_set_speed_b(&dev, 200);
    assert(enb_duty == 200);

    /* Exemplo: giro completo para cada lado */
    l298n_set_speed_a(&dev, 200);
    l298n_set_speed_b(&dev, 200);
    l298n_motor_a_set(&dev, L298N_FORWARD);
    l298n_motor_b_set(&dev, L298N_REVERSE);
    assert(in1_state && !in2_state && !in3_state && in4_state);
    l298n_motor_a_set(&dev, L298N_REVERSE);
    l298n_motor_b_set(&dev, L298N_FORWARD);
    assert(!in1_state && in2_state && in3_state && !in4_state);
    l298n_motor_a_set(&dev, L298N_BRAKE);
    l298n_motor_b_set(&dev, L298N_BRAKE);
    assert(in1_state && in2_state && in3_state && in4_state);

    printf("[motor] testes OK\n");
    return 0;
}
