#include <assert.h>
#include "Segments/max7219.h"
#include "Segments/tm1637.h"
#include "Segments/segments_examples.h"

int main(void) {
    (void)max7219_init;
    (void)max7219_set_intensity;
    (void)max7219_display_digit;
    (void)max7219_display_number;
    (void)max7219_clear;
    (void)max7219_example;
    (void)tm1637_init;
    (void)tm1637_set_brightness;
    (void)tm1637_display_segments;
    (void)tm1637_display_digits;
    (void)tm1637_display_number;
    (void)tm1637_clear;
    (void)tm1637_example;
    return 0;
}
