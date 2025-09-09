#ifndef ILI9486_H
#define ILI9486_H
#include <stdint.h>

void ili9486_init_spi(void);
void ili9486_init_parallel(void);
void ili9486_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9486_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9486_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * ili9486_init_spi();
 * ili9486_draw_pixel(0, 0, 0xFFFF);
 * ili9486_draw_rect(1, 1, 10, 10, 0xF800);
 * ili9486_draw_image(0, 0, some_pixels, 2, 2);
 * ili9486_init_parallel();
 * ili9486_draw_pixel(1, 1, 0x0000);
 */

#endif /* ILI9486_H */
