#include "keyboard.h"
#include "screen.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint16_t buffer_head = 0, buffer_tail = 0;
static int keyboard_initialized = 0;

static const char scancode_table[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void keyboard_init(void) {
    buffer_head = 0;
    buffer_tail = 0;
    keyboard_initialized = 1;
}

char keyboard_read(void) {
    if (buffer_head == buffer_tail) return 0;
    
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

int keyboard_has_input(void) {
    return buffer_head != buffer_tail;
}

char keyboard_get_last_key(void) {
    if (buffer_head == buffer_tail) return 0;
    
    // Get the last character without removing it from buffer
    uint16_t last_pos = (buffer_head - 1 + KEYBOARD_BUFFER_SIZE) % KEYBOARD_BUFFER_SIZE;
    return keyboard_buffer[last_pos];
}

void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    if (scancode < 128) {
        char c = scancode_table[scancode];
        if (c != 0) {
            keyboard_buffer[buffer_head] = c;
            buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        }
    }
}
