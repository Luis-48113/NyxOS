#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "fb.h"
#include "../../include/multiboot.h"

/* Color structure */
typedef struct {
    uint8_t r, g, b;
} color_t;

/* Predefined colors */
#define COLOR_BLACK   ((color_t){0, 0, 0})
#define COLOR_WHITE   ((color_t){255, 255, 255})
#define COLOR_RED     ((color_t){255, 0, 0})
#define COLOR_GREEN   ((color_t){0, 255, 0})
#define COLOR_BLUE    ((color_t){0, 0, 255})

void graphics_init(multiboot_info_t *mboot_info);
void graphics_clear(void);
void graphics_draw_pixel(uint32_t x, uint32_t y, color_t color);
void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, color_t color);
void graphics_draw_char(uint32_t x, uint32_t y, char c, color_t fg, color_t bg);
void graphics_draw_string(uint32_t x, uint32_t y, const char *str, color_t fg, color_t bg);
int graphics_has_framebuffer(void);
uint32_t graphics_get_width(void);
uint32_t graphics_get_height(void);

#endif /* GRAPHICS_H */
