#include "Motor/tmc2208.h"

static void ms_pins(tmc2208_t *dev, bool m1, bool m2) {
    if (dev->pins.ms1)
        dev->pins.ms1(m1);
    if (dev->pins.ms2)
        dev->pins.ms2(m2);
}

void tmc2208_init(tmc2208_t *dev, const tmc2208_pins_t *pins) {
    dev->pins = *pins;
    tmc2208_enable(dev, true);
    tmc2208_set_dir(dev, false);
    tmc2208_set_microstep(dev, TMC2208_FULL);
}

void tmc2208_step(tmc2208_t *dev) {
    if (dev->pins.step)
        dev->pins.step();
}

void tmc2208_set_dir(tmc2208_t *dev, bool dir) {
    if (dev->pins.dir)
        dev->pins.dir(dir);
}

void tmc2208_enable(tmc2208_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(!en);
}

void tmc2208_set_microstep(tmc2208_t *dev, enum tmc2208_microstep ms) {
    switch (ms) {
    case TMC2208_FULL:         ms_pins(dev, false, false); break;
    case TMC2208_HALF:         ms_pins(dev, true,  false); break;
    case TMC2208_QUARTER:      ms_pins(dev, false, true);  break;
    case TMC2208_EIGHTH:       ms_pins(dev, true,  true);  break;
    default:                   ms_pins(dev, true,  true);  break;
    }
}
