#include "Motor/l298n.h"

static void set_state(l298n_gpio_write_t in1, l298n_gpio_write_t in2,
                      enum l298n_motor_state state) {
    switch (state) {
    case L298N_FORWARD:
        in1(true);
        in2(false);
        break;
    case L298N_REVERSE:
        in1(false);
        in2(true);
        break;
    case L298N_BRAKE:
        in1(true);
        in2(true);
        break;
    case L298N_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void l298n_init(l298n_t *dev, const l298n_pins_t *pins) {
    dev->pins = *pins;
    /* Estado inicial: motores parados */
    set_state(dev->pins.in1, dev->pins.in2, L298N_COAST);
    set_state(dev->pins.in3, dev->pins.in4, L298N_COAST);
    if (dev->pins.ena)
        dev->pins.ena(0);
    if (dev->pins.enb)
        dev->pins.enb(0);
}

void l298n_motor_a_set(l298n_t *dev, enum l298n_motor_state state) {
    set_state(dev->pins.in1, dev->pins.in2, state);
}

void l298n_motor_b_set(l298n_t *dev, enum l298n_motor_state state) {
    set_state(dev->pins.in3, dev->pins.in4, state);
}

void l298n_set_speed_a(l298n_t *dev, uint8_t duty) {
    if (dev->pins.ena)
        dev->pins.ena(duty);
}

void l298n_set_speed_b(l298n_t *dev, uint8_t duty) {
    if (dev->pins.enb)
        dev->pins.enb(duty);
}
