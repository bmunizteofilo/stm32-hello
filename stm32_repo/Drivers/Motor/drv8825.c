#include "Motor/drv8825.h"

static void ms_pins(drv8825_t *dev, bool m1, bool m2, bool m3) {
    if (dev->pins.ms1)
        dev->pins.ms1(m1);
    if (dev->pins.ms2)
        dev->pins.ms2(m2);
    if (dev->pins.ms3)
        dev->pins.ms3(m3);
}

void drv8825_init(drv8825_t *dev, const drv8825_pins_t *pins) {
    dev->pins = *pins;
    drv8825_enable(dev, true);
    drv8825_set_dir(dev, false);
    drv8825_set_microstep(dev, DRV8825_FULL);
}

void drv8825_step(drv8825_t *dev) {
    if (dev->pins.step)
        dev->pins.step();
}

void drv8825_set_dir(drv8825_t *dev, bool dir) {
    if (dev->pins.dir)
        dev->pins.dir(dir);
}

void drv8825_enable(drv8825_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(!en);
}

void drv8825_set_microstep(drv8825_t *dev, enum drv8825_microstep ms) {
    switch (ms) {
    case DRV8825_FULL:          ms_pins(dev, false, false, false); break;
    case DRV8825_HALF:          ms_pins(dev, true,  false, false); break;
    case DRV8825_QUARTER:       ms_pins(dev, false, true,  false); break;
    case DRV8825_EIGHTH:        ms_pins(dev, true,  true,  false); break;
    case DRV8825_SIXTEENTH:     ms_pins(dev, false, false, true); break;
    case DRV8825_THIRTYSECOND:  ms_pins(dev, true,  false, true); break;
    }
}
