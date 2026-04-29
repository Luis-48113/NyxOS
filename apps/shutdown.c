#include "../shell/commands.h"
#include "../kernel/drivers/screen.h"
#include "../kernel/mm/memory.h"
#include "../kernel/fs/fs.h"
#include "../lib/string.h"

int cmd_version(int argc, char **argv) {
    (void)argc;
    (void)argv;
    screen_write_string("NyxOS 1.0.0-STABLE\n");
    return 0;
}

int cmd_uname(int argc, char **argv) {
    (void)argc;
    (void)argv;
    screen_write_string("NyxOS 1.0.0 (x86) #1 32-bit\n");
    return 0;
}

int cmd_whoami(int argc, char **argv) {
    (void)argc;
    (void)argv;
    screen_write_string("root\n");
    return 0;
}

int cmd_ls(int argc, char **argv) {
    (void)argc;
    (void)argv;
    fs_list();
    return 0;
}

int cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        screen_write_string("Usage: cat <file>\n");
        return -1;
    }
    
    size_t len;
    const char *content = fs_read(argv[1], &len);
    if (content) {
        for (size_t i = 0; i < len; i++) {
            screen_put_char(content[i], 0x0F);
        }
        screen_write_string("\n");
        return 0;
    }
    
    screen_write_string("File not found: ");
    screen_write_string(argv[1]);
    screen_write_string("\n");
    return -1;
}

int cmd_meminfo(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    size_t total, used, free_mem;
    mem_get_stats(&total, &used, &free_mem);
    
    screen_write_string("Memory Info:\n");
    screen_write_string("Total:  ");
    screen_write_string("128 KB\n");
    screen_write_string("Used:   varies\n");
    screen_write_string("Free:   varies\n");
    return 0;
}

int cmd_shutdown(int argc, char **argv) {
    (void)argc;
    (void)argv;
    screen_write_string("Shutting down...\n");
    __asm__ volatile("hlt");
    return 0;
}
