#include "Motor/l293d.h"

static void set_state(l293d_gpio_write_t in1, l293d_gpio_write_t in2,
                      enum l293d_motor_state state) {
    switch (state) {
    case L293D_FORWARD:
        in1(true);
        in2(false);
        break;
    case L293D_REVERSE:
        in1(false);
        in2(true);
        break;
    case L293D_BRAKE:
        in1(true);
        in2(true);
        break;
    case L293D_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void l293d_init(l293d_t *dev, const l293d_pins_t *pins) {
    dev->pins = *pins;
    /* Estado inicial: motores parados */
    set_state(dev->pins.in1, dev->pins.in2, L293D_COAST);
    set_state(dev->pins.in3, dev->pins.in4, L293D_COAST);
    if (dev->pins.ena)
        dev->pins.ena(0);
    if (dev->pins.enb)
        dev->pins.enb(0);
}

void l293d_motor_a_set(l293d_t *dev, enum l293d_motor_state state) {
    set_state(dev->pins.in1, dev->pins.in2, state);
}

void l293d_motor_b_set(l293d_t *dev, enum l293d_motor_state state) {
    set_state(dev->pins.in3, dev->pins.in4, state);
}

void l293d_set_speed_a(l293d_t *dev, uint8_t duty) {
    if (dev->pins.ena)
        dev->pins.ena(duty);
}

void l293d_set_speed_b(l293d_t *dev, uint8_t duty) {
    if (dev->pins.enb)
        dev->pins.enb(duty);
}
