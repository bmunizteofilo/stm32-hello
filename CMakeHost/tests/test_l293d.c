#include <assert.h>
#include <stdio.h>
#include "Motor/l293d.h"

static bool in1_state, in2_state, in3_state, in4_state;
static uint8_t ena_duty, enb_duty;

static void in1_write(bool level) { in1_state = level; }
static void in2_write(bool level) { in2_state = level; }
static void in3_write(bool level) { in3_state = level; }
static void in4_write(bool level) { in4_state = level; }
static void ena_pwm(uint8_t duty) { ena_duty = duty; }
static void enb_pwm(uint8_t duty) { enb_duty = duty; }

int main(void) {
    l293d_t dev;
    l293d_pins_t pins = {
        .in1 = in1_write,
        .in2 = in2_write,
        .ena = ena_pwm,
        .in3 = in3_write,
        .in4 = in4_write,
        .enb = enb_pwm,
    };
    l293d_init(&dev, &pins);
    assert(!in1_state && !in2_state && !in3_state && !in4_state);
    assert(ena_duty == 0 && enb_duty == 0);

    l293d_motor_a_set(&dev, L293D_FORWARD);
    assert(in1_state && !in2_state);
    l293d_motor_a_set(&dev, L293D_REVERSE);
    assert(!in1_state && in2_state);

    l293d_set_speed_a(&dev, 100);
    assert(ena_duty == 100);

    l293d_motor_b_set(&dev, L293D_BRAKE);
    assert(in3_state && in4_state);
    l293d_set_speed_b(&dev, 150);
    assert(enb_duty == 150);

    /* Exemplo: giro completo para cada lado */
    l293d_set_speed_a(&dev, 180);
    l293d_set_speed_b(&dev, 180);
    l293d_motor_a_set(&dev, L293D_FORWARD);
    l293d_motor_b_set(&dev, L293D_REVERSE);
    assert(in1_state && !in2_state && !in3_state && in4_state);
    l293d_motor_a_set(&dev, L293D_REVERSE);
    l293d_motor_b_set(&dev, L293D_FORWARD);
    assert(!in1_state && in2_state && in3_state && !in4_state);
    l293d_motor_a_set(&dev, L293D_BRAKE);
    l293d_motor_b_set(&dev, L293D_BRAKE);
    assert(in1_state && in2_state && in3_state && in4_state);

    printf("[l293d] testes OK\n");
    return 0;
}
