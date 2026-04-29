#include "shell.h"
#include "commands.h"
#include "../kernel/drivers/screen.h"
#include "../kernel/drivers/keyboard.h"
#include "../lib/string.h"

#define SHELL_BUFFER_SIZE 256

static char input_buffer[SHELL_BUFFER_SIZE];
static int input_pos = 0;

void shell_init(void) {
    screen_clear();
    screen_write_string("NyxOS v1-STABLE Shell\n");
    screen_write_string("Type 'help' for commands\n\n");
    commands_init();
}

void shell_run(void) {
    screen_write_string("$ ");
    input_pos = 0;
    
    while (1) {
        if (keyboard_has_input()) {
            char c = keyboard_read();
            
            if (c == '\n') {
                input_buffer[input_pos] = '\0';
                if (input_pos > 0) {
                    screen_write_string("\n");
                    commands_execute(input_buffer);
                    screen_write_string("\n");
                }
                screen_write_string("$ ");
                input_pos = 0;
            } else if (c == '\b') {
                if (input_pos > 0) {
                    input_pos--;
                    screen_write_string("\b \b");
                }
            } else if (c >= 32 && c < 127) {
                if (input_pos < SHELL_BUFFER_SIZE - 1) {
                    input_buffer[input_pos++] = c;
                    screen_put_char(c, 0x0F);
                }
            }
        }
    }
}
