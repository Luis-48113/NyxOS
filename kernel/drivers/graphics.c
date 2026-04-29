#include "graphics.h"
#include "../../kernel/kernel.h"
#include "../../lib/string.h"

static int fb_available = 0;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;

void graphics_init(multiboot_info_t *mboot_info) {
    if (!mboot_info) return;

    /* Check if framebuffer info is available (bit 12 in flags) */
    if ((mboot_info->flags & (1 << 12)) == 0) {
        return;
    }

    /* Get framebuffer info from multiboot structure */
    uint32_t fb_addr = (uint32_t)mboot_info->framebuffer_addr;
    uint32_t width = mboot_info->framebuffer_width;
    uint32_t height = mboot_info->framebuffer_height;
    uint32_t pitch = mboot_info->framebuffer_pitch;
    uint8_t bpp = mboot_info->framebuffer_bpp;

    /* Validate framebuffer parameters */
    if (fb_addr == 0 || width == 0 || height == 0) {
        return;
    }

    /* Initialize framebuffer */
    fb_init(fb_addr, width, height, pitch, bpp);
    fb_available = 1;
    fb_width = width;
    fb_height = height;
}

void graphics_clear(void) {
    if (fb_available) {
        fb_clear(COLOR_BLACK);
    }
}

void graphics_draw_pixel(uint32_t x, uint32_t y, color_t color) {
    if (fb_available) {
        fb_put_pixel(x, y, color);
    }
}

void graphics_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, color_t color) {
    if (fb_available) {
        fb_fill_rect(x, y, w, h, color);
    }
}

void graphics_draw_char(uint32_t x, uint32_t y, char c, color_t fg, color_t bg) {
    if (fb_available) {
        fb_draw_char(x, y, c, fg, bg);
    }
}

void graphics_draw_string(uint32_t x, uint32_t y, const char *str, color_t fg, color_t bg) {
    if (fb_available) {
        fb_write_string(x, y, str, fg, bg);
    }
}

int graphics_has_framebuffer(void) {
    return fb_available;
}

uint32_t graphics_get_width(void) {
    return fb_width;
}

uint32_t graphics_get_height(void) {
    return fb_height;
}
