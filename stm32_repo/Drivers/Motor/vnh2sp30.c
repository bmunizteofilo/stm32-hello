#include "Motor/vnh2sp30.h"

static void set_state(vnh2sp30_gpio_write_t in_a, vnh2sp30_gpio_write_t in_b,
                      enum vnh2sp30_state st) {
    switch (st) {
    case VNH2SP30_FORWARD:
        in_a(true);
        in_b(false);
        break;
    case VNH2SP30_REVERSE:
        in_a(false);
        in_b(true);
        break;
    case VNH2SP30_BRAKE:
        in_a(true);
        in_b(true);
        break;
    case VNH2SP30_COAST:
    default:
        in_a(false);
        in_b(false);
        break;
    }
}

void vnh2sp30_init(vnh2sp30_t *dev, const vnh2sp30_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.in_a, dev->pins.in_b, VNH2SP30_COAST);
    if (dev->pins.pwm)
        dev->pins.pwm(0);
    if (dev->pins.enable)
        dev->pins.enable(true);
}

void vnh2sp30_set(vnh2sp30_t *dev, enum vnh2sp30_state st) {
    set_state(dev->pins.in_a, dev->pins.in_b, st);
}

void vnh2sp30_set_speed(vnh2sp30_t *dev, uint8_t duty) {
    if (dev->pins.pwm)
        dev->pins.pwm(duty);
}

void vnh2sp30_enable(vnh2sp30_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(en);
}
