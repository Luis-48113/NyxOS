#include "../shell/commands.h"
#include "../kernel/drivers/screen.h"

int cmd_echo(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        screen_write_string(argv[i]);
        if (i < argc - 1) screen_write_string(" ");
    }
    screen_write_string("\n");
    return 0;
}
