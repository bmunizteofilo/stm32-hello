#include "Motor/drv8833.h"

static void set_state(drv8833_gpio_write_t in1, drv8833_gpio_write_t in2,
                      enum drv8833_state st) {
    switch (st) {
    case DRV8833_FORWARD:
        in1(true);
        in2(false);
        break;
    case DRV8833_REVERSE:
        in1(false);
        in2(true);
        break;
    case DRV8833_BRAKE:
        in1(true);
        in2(true);
        break;
    case DRV8833_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void drv8833_init(drv8833_t *dev, const drv8833_pins_t *pins) {
    dev->pins = *pins;
    set_state(dev->pins.ain1, dev->pins.ain2, DRV8833_COAST);
    set_state(dev->pins.bin1, dev->pins.bin2, DRV8833_COAST);
    if (dev->pins.pwma)
        dev->pins.pwma(0);
    if (dev->pins.pwmb)
        dev->pins.pwmb(0);
    if (dev->pins.sleep)
        dev->pins.sleep(true);
}

void drv8833_motor_a_set(drv8833_t *dev, enum drv8833_state st) {
    set_state(dev->pins.ain1, dev->pins.ain2, st);
}

void drv8833_motor_b_set(drv8833_t *dev, enum drv8833_state st) {
    set_state(dev->pins.bin1, dev->pins.bin2, st);
}

void drv8833_set_speed_a(drv8833_t *dev, uint8_t duty) {
    if (dev->pins.pwma)
        dev->pins.pwma(duty);
}

void drv8833_set_speed_b(drv8833_t *dev, uint8_t duty) {
    if (dev->pins.pwmb)
        dev->pins.pwmb(duty);
}

void drv8833_sleep(drv8833_t *dev, bool sleep) {
    if (dev->pins.sleep)
        dev->pins.sleep(!sleep ? true : false); /* sleep=true desliga */
}
