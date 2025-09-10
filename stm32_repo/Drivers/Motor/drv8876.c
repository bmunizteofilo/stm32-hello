#include "Motor/drv8876.h"

static void set_state(drv8876_gpio_write_t in1, drv8876_gpio_write_t in2,
                      enum drv8876_state st) {
    switch (st) {
    case DRV8876_FORWARD:
        in1(true);
        in2(false);
        break;
    case DRV8876_REVERSE:
        in1(false);
        in2(true);
        break;
    case DRV8876_BRAKE:
        in1(true);
        in2(true);
        break;
    case DRV8876_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void drv8876_init(drv8876_t *dev, const drv8876_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.in1, dev->pins.in2, DRV8876_COAST);
    if (dev->pins.pwm)
        dev->pins.pwm(0);
    if (dev->pins.sleep)
        dev->pins.sleep(true);
}

void drv8876_set(drv8876_t *dev, enum drv8876_state st) {
    set_state(dev->pins.in1, dev->pins.in2, st);
}

void drv8876_set_speed(drv8876_t *dev, uint8_t duty) {
    if (dev->pins.pwm)
        dev->pins.pwm(duty);
}

void drv8876_sleep(drv8876_t *dev, bool sleep) {
    if (dev->pins.sleep)
        dev->pins.sleep(!sleep);
}
