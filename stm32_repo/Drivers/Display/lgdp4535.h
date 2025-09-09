#ifndef LGDP4535_H
#define LGDP4535_H
#include <stdint.h>

void lgdp4535_init_spi(void);
void lgdp4535_init_parallel(void);
void lgdp4535_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void lgdp4535_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lgdp4535_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * lgdp4535_init_spi();
 * lgdp4535_draw_pixel(0, 0, 0xFFFF);
 * lgdp4535_draw_rect(1, 1, 10, 10, 0xF800);
 * lgdp4535_draw_image(0, 0, some_pixels, 2, 2);
 * lgdp4535_init_parallel();
 * lgdp4535_draw_pixel(1, 1, 0x0000);
 */

#endif /* LGDP4535_H */
