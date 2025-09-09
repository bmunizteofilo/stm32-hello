#ifndef R61509_H
#define R61509_H
#include <stdint.h>

void r61509_init_spi(void);
void r61509_init_parallel(void);
void r61509_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void r61509_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void r61509_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * r61509_init_spi();
 * r61509_draw_pixel(0, 0, 0xFFFF);
 * r61509_draw_rect(1, 1, 10, 10, 0xF800);
 * r61509_draw_image(0, 0, some_pixels, 2, 2);
 * r61509_init_parallel();
 * r61509_draw_pixel(1, 1, 0x0000);
 */

#endif /* R61509_H */
