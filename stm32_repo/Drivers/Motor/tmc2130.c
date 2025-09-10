#include "Motor/tmc2130.h"

static void spi_send(tmc2130_t *dev, uint32_t val) {
    if (dev->pins.cs)
        dev->pins.cs(false);
    for (int i = 31; i >= 0; --i) {
        if (dev->pins.sdi)
            dev->pins.sdi((val >> i) & 1);
        if (dev->pins.sck) {
            dev->pins.sck(true);
            dev->pins.sck(false);
        }
    }
    if (dev->pins.cs)
        dev->pins.cs(true);
}

void tmc2130_init(tmc2130_t *dev, const tmc2130_pins_t *pins) {
    dev->pins = *pins;
    tmc2130_enable(dev, true);
    tmc2130_set_dir(dev, false);
    tmc2130_set_microstep(dev, TMC2130_FULL);
}

void tmc2130_step(tmc2130_t *dev) {
    if (dev->pins.step)
        dev->pins.step();
}

void tmc2130_set_dir(tmc2130_t *dev, bool dir) {
    if (dev->pins.dir)
        dev->pins.dir(dir);
}

void tmc2130_enable(tmc2130_t *dev, bool en) {
    if (dev->pins.enable)
        dev->pins.enable(!en);
}

void tmc2130_set_microstep(tmc2130_t *dev, enum tmc2130_microstep ms) {
    spi_send(dev, (uint32_t)ms);
}
