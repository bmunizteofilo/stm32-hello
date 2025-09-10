#include "Motor/a4988.h"

static void ms_pins(a4988_t *dev, bool m1, bool m2, bool m3) {
    if (dev->pins.ms1)
        dev->pins.ms1(m1);
    if (dev->pins.ms2)
        dev->pins.ms2(m2);
    if (dev->pins.ms3)
        dev->pins.ms3(m3);
}

void a4988_init(a4988_t *dev, const a4988_pins_t *pins) {
    dev->pins = *pins;
    a4988_enable(dev, true);
    a4988_set_dir(dev, false);
    a4988_set_microstep(dev, A4988_FULL);
}

void a4988_step(a4988_t *dev) {
    if (dev->pins.step)
        dev->pins.step();
}

void a4988_set_dir(a4988_t *dev, bool dir) {
    if (dev->pins.dir)
        dev->pins.dir(dir);
}

void a4988_enable(a4988_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(!en); /* low = enable */
}

void a4988_set_microstep(a4988_t *dev, enum a4988_microstep ms) {
    switch (ms) {
    case A4988_FULL:    ms_pins(dev, false, false, false); break;
    case A4988_HALF:    ms_pins(dev, true,  false, false); break;
    case A4988_QUARTER: ms_pins(dev, false, true,  false); break;
    case A4988_EIGHTH:  ms_pins(dev, true,  true,  false); break;
    case A4988_SIXTEENTH: ms_pins(dev, true,  true,  true); break;
    }
}
