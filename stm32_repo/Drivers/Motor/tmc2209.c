#include "Motor/tmc2209.h"

static void ms_pins(tmc2209_t *dev, bool m1, bool m2) {
    if (dev->pins.ms1)
        dev->pins.ms1(m1);
    if (dev->pins.ms2)
        dev->pins.ms2(m2);
}

void tmc2209_init(tmc2209_t *dev, const tmc2209_pins_t *pins) {
    dev->pins = *pins;
    tmc2209_enable(dev, true);
    tmc2209_set_dir(dev, false);
    tmc2209_set_microstep(dev, TMC2209_FULL);
}

void tmc2209_step(tmc2209_t *dev) {
    if (dev->pins.step)
        dev->pins.step();
}

void tmc2209_set_dir(tmc2209_t *dev, bool dir) {
    if (dev->pins.dir)
        dev->pins.dir(dir);
}

void tmc2209_enable(tmc2209_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(!en);
}

void tmc2209_set_microstep(tmc2209_t *dev, enum tmc2209_microstep ms) {
    switch (ms) {
    case TMC2209_FULL:         ms_pins(dev, false, false); break;
    case TMC2209_HALF:         ms_pins(dev, true,  false); break;
    case TMC2209_QUARTER:      ms_pins(dev, false, true);  break;
    case TMC2209_EIGHTH:       ms_pins(dev, true,  true);  break;
    default:                   ms_pins(dev, true,  true);  break;
    }
}
