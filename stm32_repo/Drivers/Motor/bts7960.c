#include "Motor/bts7960.h"

void bts7960_init(bts7960_t *dev, const bts7960_pins_t *pins) {
    dev->pins = *pins;
    dev->state = BTS7960_COAST;
    dev->speed = 0;
    if (dev->pins.rpwm)
        dev->pins.rpwm(0);
    if (dev->pins.lpwm)
        dev->pins.lpwm(0);
    if (dev->pins.ren)
        dev->pins.ren(true);
    if (dev->pins.len)
        dev->pins.len(true);
}

void bts7960_set(bts7960_t *dev, enum bts7960_state st) {
    dev->state = st;
    switch (st) {
    case BTS7960_FORWARD:
        if (dev->pins.rpwm)
            dev->pins.rpwm(dev->speed);
        if (dev->pins.lpwm)
            dev->pins.lpwm(0);
        break;
    case BTS7960_REVERSE:
        if (dev->pins.lpwm)
            dev->pins.lpwm(dev->speed);
        if (dev->pins.rpwm)
            dev->pins.rpwm(0);
        break;
    case BTS7960_BRAKE:
        if (dev->pins.rpwm)
            dev->pins.rpwm(255);
        if (dev->pins.lpwm)
            dev->pins.lpwm(255);
        break;
    case BTS7960_COAST:
    default:
        if (dev->pins.rpwm)
            dev->pins.rpwm(0);
        if (dev->pins.lpwm)
            dev->pins.lpwm(0);
        break;
    }
}

void bts7960_set_speed(bts7960_t *dev, uint8_t duty) {
    dev->speed = duty;
    bts7960_set(dev, dev->state);
}

void bts7960_enable(bts7960_t *dev, bool en) {
    if (dev->pins.ren)
        dev->pins.ren(en);
    if (dev->pins.len)
        dev->pins.len(en);
}
