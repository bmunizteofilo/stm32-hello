#include "Motor/tb6612fng.h"

static void set_state(tb6612fng_gpio_write_t in1, tb6612fng_gpio_write_t in2,
                      enum tb6612fng_motor_state state) {
    switch (state) {
    case TB6612FNG_FORWARD:
        in1(true);
        in2(false);
        break;
    case TB6612FNG_REVERSE:
        in1(false);
        in2(true);
        break;
    case TB6612FNG_BRAKE:
        in1(true);
        in2(true);
        break;
    case TB6612FNG_COAST:
    default:
        in1(false);
        in2(false);
        break;
    }
}

void tb6612fng_init(tb6612fng_t *dev, const tb6612fng_pins_t *pins) {
    dev->pins = *pins;
    /* Habilita sa\xc3\xadda */
    if (dev->pins.stby)
        dev->pins.stby(true);
    /* Motores parados inicialmente */
    set_state(dev->pins.ain1, dev->pins.ain2, TB6612FNG_COAST);
    set_state(dev->pins.bin1, dev->pins.bin2, TB6612FNG_COAST);
    if (dev->pins.pwma)
        dev->pins.pwma(0);
    if (dev->pins.pwmb)
        dev->pins.pwmb(0);
}

void tb6612fng_motor_a_set(tb6612fng_t *dev, enum tb6612fng_motor_state state) {
    set_state(dev->pins.ain1, dev->pins.ain2, state);
}

void tb6612fng_motor_b_set(tb6612fng_t *dev, enum tb6612fng_motor_state state) {
    set_state(dev->pins.bin1, dev->pins.bin2, state);
}

void tb6612fng_set_speed_a(tb6612fng_t *dev, uint8_t duty) {
    if (dev->pins.pwma)
        dev->pins.pwma(duty);
}

void tb6612fng_set_speed_b(tb6612fng_t *dev, uint8_t duty) {
    if (dev->pins.pwmb)
        dev->pins.pwmb(duty);
}

void tb6612fng_standby(tb6612fng_t *dev, bool standby) {
    if (dev->pins.stby)
        dev->pins.stby(!standby); /* STBY alto = ativo */
}
