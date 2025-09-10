#include "Motor/drv8871.h"

static void set_state(drv8871_gpio_write_t in1, drv8871_gpio_write_t in2,
                      enum drv8871_state st) {
    switch (st) {
    case DRV8871_FORWARD:
        in1(true);
        in2(false);
        break;
    case DRV8871_REVERSE:
        in1(false);
        in2(true);
        break;
    case DRV8871_BRAKE:
        in1(true);
        in2(true);
        break;
    case DRV8871_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void drv8871_init(drv8871_t *dev, const drv8871_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.in1, dev->pins.in2, DRV8871_COAST);
    if (dev->pins.pwm)
        dev->pins.pwm(0);
    if (dev->pins.sleep)
        dev->pins.sleep(true);
}

void drv8871_set(drv8871_t *dev, enum drv8871_state st) {
    set_state(dev->pins.in1, dev->pins.in2, st);
}

void drv8871_set_speed(drv8871_t *dev, uint8_t duty) {
    if (dev->pins.pwm)
        dev->pins.pwm(duty);
}

void drv8871_sleep(drv8871_t *dev, bool sleep) {
    if (dev->pins.sleep)
        dev->pins.sleep(!sleep);
}
