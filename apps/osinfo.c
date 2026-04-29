#include "../shell/commands.h"
#include "../kernel/drivers/screen.h"
#include "../kernel/cpu/cpu.h"
#include "../kernel/drivers/graphics.h"
#include "../lib/string.h"

int cmd_osinfo(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    cpu_init();
    cpu_info_t *info = cpu_get_info();
    
    screen_write_string("=== NyxOS v1-STABLE ===\n");
    screen_write_string("CPU Vendor:  ");
    screen_write_string(info->vendor);
    screen_write_string("\n");
    screen_write_string("CPU Brand:   ");
    screen_write_string(info->brand);
    screen_write_string("\n");
    screen_write_string("CPU Family:  ");
    char buf[10];
    screen_write_string("\n");
    screen_write_string("Mode:        32-bit x86\n");
    screen_write_string("Boot:        Multiboot\n");
    
    return 0;
}
