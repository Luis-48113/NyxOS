#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define SCREEN_WIDTH   80
#define SCREEN_HEIGHT  25
#define VGA_MEMORY     0xB8000

void screen_init(void);
void screen_put_char(char c, uint8_t color);
void screen_clear(void);
void screen_set_cursor(uint8_t x, uint8_t y);
void screen_write_string(const char *s);
void screen_printf(const char *fmt, ...);
void screen_newline(void);
static void screen_clear_vga(void);

#endif /* SCREEN_H */
