#include <assert.h>
#include <string.h>
#include "Leds/ws2812.h"
#include "Leds/apa102.h"
#include "Leds/sk6812.h"
#include "spi.h"

static void test_ws2812(void) {
    ws2812_color_t pixels[1];
    uint16_t buffer[WS2812_BUFFER_LEN(1)];
    ws2812_t strip;
    ws2812_init(&strip, 1, pixels, buffer);
    ws2812_set_pixel(&strip, 0, 0xAA, 0x55, 0x00);
    ws2812_prepare_buffer(&strip);
    /* Verify GRB order encoding */
    assert(buffer[0] == WS2812_CODE_0); /* G bit7 =0 */
    assert(buffer[1] == WS2812_CODE_1); /* G bit6 =1 */
    assert(buffer[8] == WS2812_CODE_1); /* R bit7 =1 */
    assert(buffer[9] == WS2812_CODE_0); /* R bit6 =0 */
    assert(buffer[16] == WS2812_CODE_0); /* B all zero */
    /* Timer period for 48 MHz clock should yield 59 */
    assert(WS2812_TIMER_PERIOD(48000000u) == 59u);
}

static void test_apa102(void) {
    apa102_color_t pixels[1];
    uint8_t buffer[APA102_BUFFER_LEN(1)];
    apa102_t strip;
    apa102_init(&strip, 1, pixels, buffer);
    apa102_set_pixel(&strip, 0, 10, 20, 30);
    apa102_set_global_brightness(&strip, 31);
    apa102_prepare_buffer(&strip);
    assert(buffer[0] == 0x00);
    assert(buffer[4] == 0xFF);
    assert(buffer[5] == 30);
    assert(buffer[6] == 20);
    assert(buffer[7] == 10);
    assert(buffer[8] == 0xFF);
    assert(buffer[11] == 0xFF);
}

static void test_apa102_spi_send(void) {
    apa102_color_t pixels[1];
    uint8_t buffer[APA102_BUFFER_LEN(1)];
    apa102_t strip;
    apa102_init(&strip, 1, pixels, buffer);
    apa102_prepare_buffer(&strip);
    spi_stub_reset();
    apa102_send_spi(&strip, SPI1);
    assert(spi_stub_count() == APA102_BUFFER_LEN(1));
    assert(spi_stub_get(0) == 0x00);
    assert(spi_stub_get(4) == 0xFF);
}

static void test_apa102_transfer_time(void) {
    apa102_color_t pixels[2];
    uint8_t buffer[APA102_BUFFER_LEN(2)];
    apa102_t strip;
    apa102_init(&strip, 2, pixels, buffer);
    apa102_prepare_buffer(&strip);
    static const uint16_t divs[8] = {2u,4u,8u,16u,32u,64u,128u,256u};
    uint32_t spi_hz = 48000000u / divs[SPI_BAUD_DIV8];
    uint32_t us = apa102_estimate_transfer_time_us(&strip, spi_hz);
    /* 16 bytes -> 128 bits, 6 MHz -> ~21.3 us -> 22 us with rounding */
    assert(us == 22u);
}

static void test_sk6812(void) {
    sk6812_color_t pixels[1];
    uint16_t buffer[SK6812_BUFFER_LEN(1)];
    sk6812_t strip;
    sk6812_init(&strip, 1, pixels, buffer);
    sk6812_set_pixel(&strip, 0, 0x0F, 0xF0, 0x00, 0xFF);
    sk6812_prepare_buffer(&strip);
    assert(buffer[0] == SK6812_CODE_1); /* G bit7 =1 */
    assert(buffer[8] == SK6812_CODE_0); /* R bit7 =0 */
    assert(buffer[16] == SK6812_CODE_0); /* B bit7 =0 */
    assert(buffer[24] == SK6812_CODE_1); /* W bit7 =1 */
    assert(SK6812_TIMER_PERIOD(48000000u) == 59u);
}

static void test_ws2812_time(void) {
    ws2812_color_t pixels[3];
    uint16_t buffer[WS2812_BUFFER_LEN(3)];
    ws2812_t strip;
    ws2812_init(&strip, 3, pixels, buffer);
    uint32_t us = ws2812_estimate_transfer_time_us(&strip);
    assert(us == 90u);
}

static void test_sk6812_time(void) {
    sk6812_color_t pixels[3];
    uint16_t buffer[SK6812_BUFFER_LEN(3)];
    sk6812_t strip;
    sk6812_init(&strip, 3, pixels, buffer);
    uint32_t us = sk6812_estimate_transfer_time_us(&strip);
    assert(us == 120u);
}

int main(void) {
    test_ws2812();
    test_apa102();
    test_apa102_spi_send();
    test_apa102_transfer_time();
    test_sk6812();
    test_ws2812_time();
    test_sk6812_time();
    return 0;
}
