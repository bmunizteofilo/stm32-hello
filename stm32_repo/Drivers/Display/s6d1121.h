#ifndef S6D1121_H
#define S6D1121_H
#include <stdint.h>

void s6d1121_init_spi(void);
void s6d1121_init_parallel(void);
void s6d1121_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void s6d1121_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void s6d1121_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * s6d1121_init_spi();
 * s6d1121_draw_pixel(0, 0, 0xFFFF);
 * s6d1121_draw_rect(1, 1, 10, 10, 0xF800);
 * s6d1121_draw_image(0, 0, some_pixels, 2, 2);
 * s6d1121_init_parallel();
 * s6d1121_draw_pixel(1, 1, 0x0000);
 */

#endif /* S6D1121_H */
