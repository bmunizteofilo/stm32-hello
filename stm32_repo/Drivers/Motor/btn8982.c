#include "Motor/btn8982.h"

static void set_state(btn8982_gpio_write_t in1, btn8982_gpio_write_t in2,
                      enum btn8982_state st) {
    switch (st) {
    case BTN8982_FORWARD:
        in1(true);
        in2(false);
        break;
    case BTN8982_REVERSE:
        in1(false);
        in2(true);
        break;
    case BTN8982_BRAKE:
        in1(true);
        in2(true);
        break;
    case BTN8982_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void btn8982_init(btn8982_t *dev, const btn8982_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.in1, dev->pins.in2, BTN8982_COAST);
    if (dev->pins.pwm)
        dev->pins.pwm(0);
    if (dev->pins.enable)
        dev->pins.enable(true);
}

void btn8982_set(btn8982_t *dev, enum btn8982_state st) {
    set_state(dev->pins.in1, dev->pins.in2, st);
}

void btn8982_set_speed(btn8982_t *dev, uint8_t duty) {
    if (dev->pins.pwm)
        dev->pins.pwm(duty);
}

void btn8982_enable(btn8982_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(en);
}
