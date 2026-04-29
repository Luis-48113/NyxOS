#ifndef FB_H
#define FB_H

#include <stdint.h>
#include "../../include/stddef.h"

typedef struct {
    uint32_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
} framebuffer_t;

typedef struct {
    uint8_t r, g, b;
} color_t;

#define COLOR_BLACK   ((color_t){0, 0, 0})
#define COLOR_WHITE   ((color_t){255, 255, 255})
#define COLOR_RED     ((color_t){255, 0, 0})
#define COLOR_GREEN   ((color_t){0, 255, 0})
#define COLOR_BLUE    ((color_t){0, 0, 255})
#define COLOR_YELLOW  ((color_t){255, 255, 0})
#define COLOR_CYAN    ((color_t){0, 255, 255})
#define COLOR_MAGENTA ((color_t){255, 0, 255})
#define COLOR_GRAY    ((color_t){128, 128, 128})

void fb_init(uint32_t addr, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp);
void fb_put_pixel(uint32_t x, uint32_t y, color_t color);
void fb_clear(color_t color);
void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, color_t color);
void fb_draw_char(uint32_t x, uint32_t y, char c, color_t fg, color_t bg);
void fb_write_string(uint32_t x, uint32_t y, const char *str, color_t fg, color_t bg);
uint32_t fb_get_width(void);
uint32_t fb_get_height(void);

#endif /* FB_H */
