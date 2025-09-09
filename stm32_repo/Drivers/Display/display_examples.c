#include "ili9341.h"
#include "ili9325.h"
#include "ssd1963.h"
#include "st7789.h"
#include "hx8357.h"
#include "ra8875.h"
#include "s6d1121.h"
#include "lgdp4535.h"
#include "ili9486.h"
#include "r61509.h"

static const uint16_t demo_img[4] = {0xFFFFu, 0x0000u, 0x0000u, 0xFFFFu};

void ili9341_example(void) {
    ili9341_init_spi();
    ili9341_draw_pixel(0, 0, 0xFFFF);
    ili9341_draw_rect(1, 1, 2, 2, 0xF800);
    ili9341_draw_image(4, 4, demo_img, 2, 2);
    ili9341_init_parallel();
}

void ili9325_example(void) {
    ili9325_init_spi();
    ili9325_draw_pixel(0, 0, 0xFFFF);
    ili9325_draw_rect(1, 1, 2, 2, 0xF800);
    ili9325_draw_image(4, 4, demo_img, 2, 2);
    ili9325_init_parallel();
}

void ssd1963_example(void) {
    ssd1963_init_spi();
    ssd1963_draw_pixel(0, 0, 0xFFFF);
    ssd1963_draw_rect(1, 1, 2, 2, 0xF800);
    ssd1963_draw_image(4, 4, demo_img, 2, 2);
    ssd1963_init_parallel();
}

void st7789_example(void) {
    st7789_init_spi();
    st7789_draw_pixel(0, 0, 0xFFFF);
    st7789_draw_rect(1, 1, 2, 2, 0xF800);
    st7789_draw_image(4, 4, demo_img, 2, 2);
    st7789_init_parallel();
}

void hx8357_example(void) {
    hx8357_init_spi();
    hx8357_draw_pixel(0, 0, 0xFFFF);
    hx8357_draw_rect(1, 1, 2, 2, 0xF800);
    hx8357_draw_image(4, 4, demo_img, 2, 2);
    hx8357_init_parallel();
}

void ra8875_example(void) {
    ra8875_init_spi();
    ra8875_draw_pixel(0, 0, 0xFFFF);
    ra8875_draw_rect(1, 1, 2, 2, 0xF800);
    ra8875_draw_image(4, 4, demo_img, 2, 2);
    ra8875_init_parallel();
}

void s6d1121_example(void) {
    s6d1121_init_spi();
    s6d1121_draw_pixel(0, 0, 0xFFFF);
    s6d1121_draw_rect(1, 1, 2, 2, 0xF800);
    s6d1121_draw_image(4, 4, demo_img, 2, 2);
    s6d1121_init_parallel();
}

void lgdp4535_example(void) {
    lgdp4535_init_spi();
    lgdp4535_draw_pixel(0, 0, 0xFFFF);
    lgdp4535_draw_rect(1, 1, 2, 2, 0xF800);
    lgdp4535_draw_image(4, 4, demo_img, 2, 2);
    lgdp4535_init_parallel();
}

void ili9486_example(void) {
    ili9486_init_spi();
    ili9486_draw_pixel(0, 0, 0xFFFF);
    ili9486_draw_rect(1, 1, 2, 2, 0xF800);
    ili9486_draw_image(4, 4, demo_img, 2, 2);
    ili9486_init_parallel();
}

void r61509_example(void) {
    r61509_init_spi();
    r61509_draw_pixel(0, 0, 0xFFFF);
    r61509_draw_rect(1, 1, 2, 2, 0xF800);
    r61509_draw_image(4, 4, demo_img, 2, 2);
    r61509_init_parallel();
}
