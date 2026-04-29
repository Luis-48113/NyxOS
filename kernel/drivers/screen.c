#include "screen.h"
#include "graphics.h"
#include "fb.h"
#include "../../lib/string.h"

static uint32_t text_cursor_x = 0;
static uint32_t text_cursor_y = 0;
static int use_framebuffer = 0;

/* VGA fallback */
static uint16_t *vga_mem = (uint16_t *)VGA_MEMORY;
static uint8_t vga_cursor_x = 0, vga_cursor_y = 0;

void screen_init(void) {
    use_framebuffer = graphics_has_framebuffer();
    
    if (use_framebuffer) {
        graphics_clear();
        text_cursor_x = 10;
        text_cursor_y = 10;
    } else {
        screen_clear_vga();
    }
}

static void screen_clear_vga(void) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_mem[i] = 0;
    }
    vga_cursor_x = 0;
    vga_cursor_y = 0;
}

void screen_put_char(char c, uint8_t color) {
    if (use_framebuffer) {
        if (c == '\n') {
            text_cursor_y += 10;
            text_cursor_x = 10;
        } else if (c == '\t') {
            text_cursor_x += 32;
        } else {
            color_t fg = {255, 255, 255};
            color_t bg = {0, 0, 0};
            graphics_draw_char(text_cursor_x, text_cursor_y, c, fg, bg);
            text_cursor_x += 8;
            
            if (text_cursor_x + 8 > 800) {
                text_cursor_y += 10;
                text_cursor_x = 10;
            }
        }
        
        if (text_cursor_y + 10 > 600) {
            text_cursor_y = 10;
        }
    } else {
        /* VGA text mode fallback */
        if (c == '\n') {
            vga_cursor_y++;
            vga_cursor_x = 0;
        } else if (c == '\r') {
            vga_cursor_x = 0;
        } else if (c == '\t') {
            vga_cursor_x += 4;
        } else if (vga_cursor_x < SCREEN_WIDTH) {
            uint16_t index = vga_cursor_y * SCREEN_WIDTH + vga_cursor_x;
            vga_mem[index] = ((color << 8) | (unsigned char)c);
            vga_cursor_x++;
        }
        
        if (vga_cursor_x >= SCREEN_WIDTH) {
            vga_cursor_x = 0;
            vga_cursor_y++;
        }
        
        if (vga_cursor_y >= SCREEN_HEIGHT) {
            for (int i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i++) {
                vga_mem[i] = vga_mem[i + SCREEN_WIDTH];
            }
            for (int i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
                vga_mem[i] = 0;
            }
            vga_cursor_y = SCREEN_HEIGHT - 1;
        }
    }
}

void screen_clear(void) {
    if (use_framebuffer) {
        graphics_clear();
        text_cursor_x = 10;
        text_cursor_y = 10;
    } else {
        screen_clear_vga();
    }
}

void screen_set_cursor(uint8_t x, uint8_t y) {
    if (use_framebuffer) {
        text_cursor_x = x;
        text_cursor_y = y;
    } else {
        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
            vga_cursor_x = x;
            vga_cursor_y = y;
        }
    }
}

void screen_write_string(const char *s) {
    if (!s) return;
    for (int i = 0; s[i] != '\0'; i++) {
        screen_put_char(s[i], 0x0F);
    }
}

void screen_printf(const char *fmt, ...) {
    if (!fmt) return;
    screen_write_string(fmt);
}

void screen_newline(void) {
    screen_put_char('\n', 0x0F);
}
