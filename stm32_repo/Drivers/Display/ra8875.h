#ifndef RA8875_H
#define RA8875_H
#include <stdint.h>

void ra8875_init_spi(void);
void ra8875_init_parallel(void);
void ra8875_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ra8875_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ra8875_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * ra8875_init_spi();
 * ra8875_draw_pixel(0, 0, 0xFFFF);
 * ra8875_draw_rect(1, 1, 10, 10, 0xF800);
 * ra8875_draw_image(0, 0, some_pixels, 2, 2);
 * ra8875_init_parallel();
 * ra8875_draw_pixel(1, 1, 0x0000);
 */

#endif /* RA8875_H */
