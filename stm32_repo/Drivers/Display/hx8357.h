#ifndef HX8357_H
#define HX8357_H
#include <stdint.h>

void hx8357_init_spi(void);
void hx8357_init_parallel(void);
void hx8357_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void hx8357_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void hx8357_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * hx8357_init_spi();
 * hx8357_draw_pixel(0, 0, 0xFFFF);
 * hx8357_draw_rect(1, 1, 10, 10, 0xF800);
 * hx8357_draw_image(0, 0, some_pixels, 2, 2);
 * hx8357_init_parallel();
 * hx8357_draw_pixel(1, 1, 0x0000);
 */

#endif /* HX8357_H */
