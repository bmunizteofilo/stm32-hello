#include "Segments/max7219.h"
#include "Segments/tm1637.h"

void max7219_example(void) {
    max7219_t disp;
    max7219_init(&disp, SPI1, GPIOA, 4u);
    max7219_set_intensity(&disp, 8u);
    max7219_display_number(&disp, 1234);
}

void tm1637_example(void) {
    tm1637_t disp;
    tm1637_init(&disp, GPIOA, 0u, GPIOA, 1u);
    tm1637_set_brightness(&disp, 4u);
    tm1637_display_number(&disp, 5678);
}
