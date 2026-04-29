#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_CTRL_PORT   0x64
#define KEYBOARD_BUFFER_SIZE 256

void keyboard_init(void);
char keyboard_read(void);
int keyboard_has_input(void);
char keyboard_get_last_key(void);
void keyboard_handler(void);

#endif /* KEYBOARD_H */
