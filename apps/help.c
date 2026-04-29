#include "../shell/commands.h"
#include "../kernel/drivers/screen.h"
#include "../lib/string.h"

int cmd_help(int argc, char **argv) {
    screen_write_string("Available commands:\n");
    screen_write_string("  help      - Show this help message\n");
    screen_write_string("  echo      - Print text\n");
    screen_write_string("  clear     - Clear screen\n");
    screen_write_string("  osinfo    - Display OS information\n");
    screen_write_string("  version   - Show OS version\n");
    screen_write_string("  uname     - Display system name\n");
    screen_write_string("  whoami    - Display current user\n");
    screen_write_string("  ls        - List filesystem\n");
    screen_write_string("  cat       - Display file contents\n");
    screen_write_string("  meminfo   - Display memory stats\n");
    screen_write_string("  shutdown  - Halt the system\n");
    return 0;
}
