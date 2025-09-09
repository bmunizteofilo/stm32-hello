#ifndef SSD1963_H
#define SSD1963_H
#include <stdint.h>

void ssd1963_init_spi(void);
void ssd1963_init_parallel(void);
void ssd1963_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ssd1963_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ssd1963_draw_image(uint16_t x, uint16_t y, const uint16_t *data, uint16_t w, uint16_t h);

/* Example:
 * ssd1963_init_spi();
 * ssd1963_draw_pixel(0, 0, 0xFFFF);
 * ssd1963_draw_rect(1, 1, 10, 10, 0xF800);
 * ssd1963_draw_image(0, 0, some_pixels, 2, 2);
 * ssd1963_init_parallel();
 * ssd1963_draw_pixel(1, 1, 0x0000);
 */

#endif /* SSD1963_H */
