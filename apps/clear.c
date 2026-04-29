#include "../shell/commands.h"
#include "../kernel/drivers/screen.h"

int cmd_clear(int argc, char **argv) {
    (void)argc;
    (void)argv;
    screen_clear();
    return 0;
}
