#ifndef ST7789_H
#define ST7789_H
#include <stdint.h>

void st7789_init_spi(void);
void st7789_init_parallel(void);
void st7789_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void st7789_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void st7789_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * st7789_init_spi();
 * st7789_draw_pixel(0, 0, 0xFFFF);
 * st7789_draw_rect(1, 1, 10, 10, 0xF800);
 * st7789_draw_image(0, 0, some_pixels, 2, 2);
 * st7789_init_parallel();
 * st7789_draw_pixel(1, 1, 0x0000);
 */

#endif /* ST7789_H */
