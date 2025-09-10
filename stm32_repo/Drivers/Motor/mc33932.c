#include "Motor/mc33932.h"

static void set_state(mc33932_gpio_write_t in1, mc33932_gpio_write_t in2,
                      enum mc33932_state st) {
    switch (st) {
    case MC33932_FORWARD:
        in1(true);
        in2(false);
        break;
    case MC33932_REVERSE:
        in1(false);
        in2(true);
        break;
    case MC33932_BRAKE:
        in1(true);
        in2(true);
        break;
    case MC33932_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void mc33932_init(mc33932_t *dev, const mc33932_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.in1, dev->pins.in2, MC33932_COAST);
    set_state(dev->pins.in3, dev->pins.in4, MC33932_COAST);
    if (dev->pins.ena)
        dev->pins.ena(0);
    if (dev->pins.enb)
        dev->pins.enb(0);
}

void mc33932_motor_a_set(mc33932_t *dev, enum mc33932_state st) {
    set_state(dev->pins.in1, dev->pins.in2, st);
}

void mc33932_motor_b_set(mc33932_t *dev, enum mc33932_state st) {
    set_state(dev->pins.in3, dev->pins.in4, st);
}

void mc33932_set_speed_a(mc33932_t *dev, uint8_t duty) {
    if (dev->pins.ena)
        dev->pins.ena(duty);
}

void mc33932_set_speed_b(mc33932_t *dev, uint8_t duty) {
    if (dev->pins.enb)
        dev->pins.enb(duty);
}
