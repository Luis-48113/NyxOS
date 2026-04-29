#include "commands.h"
#include "../kernel/drivers/screen.h"
#include "../lib/string.h"

#define MAX_COMMANDS 32

static command_t command_registry[MAX_COMMANDS];
static int command_count = 0;

extern int cmd_help(int argc, char **argv);
extern int cmd_echo(int argc, char **argv);
extern int cmd_clear(int argc, char **argv);
extern int cmd_osinfo(int argc, char **argv);
extern int cmd_version(int argc, char **argv);
extern int cmd_uname(int argc, char **argv);
extern int cmd_whoami(int argc, char **argv);
extern int cmd_ls(int argc, char **argv);
extern int cmd_cat(int argc, char **argv);
extern int cmd_meminfo(int argc, char **argv);
extern int cmd_shutdown(int argc, char **argv);
extern int cmd_snake(int argc, char **argv);

void commands_init(void) {
    command_count = 0;
    
    commands_register("help", "Display help", cmd_help);
    commands_register("echo", "Echo text", cmd_echo);
    commands_register("clear", "Clear screen", cmd_clear);
    commands_register("osinfo", "Display OS info", cmd_osinfo);
    commands_register("version", "Display version", cmd_version);
    commands_register("uname", "Display system info", cmd_uname);
    commands_register("whoami", "Display current user", cmd_whoami);
    commands_register("ls", "List files", cmd_ls);
    commands_register("cat", "Display file contents", cmd_cat);
    commands_register("meminfo", "Display memory info", cmd_meminfo);
    commands_register("shutdown", "Shutdown system", cmd_shutdown);
    commands_register("snake", "Play snake game", cmd_snake);
}

void commands_register(const char *name, const char *help, command_func_t func) {
    if (command_count >= MAX_COMMANDS) return;
    
    command_registry[command_count].name = name;
    command_registry[command_count].help = help;
    command_registry[command_count].func = func;
    command_count++;
}

int commands_execute(const char *cmdline) {
    if (!cmdline) return -1;
    
    char cmd_copy[256];
    strncpy(cmd_copy, cmdline, 255);
    cmd_copy[255] = '\0';
    
    char *argv[16] = {NULL};
    int argc = 0;
    
    char *p = cmd_copy;
    char *start = NULL;
    int in_arg = 0;
    
    while (*p != '\0' && argc < 16) {
        if (*p == ' ') {
            if (in_arg) {
                *p = '\0';
                argv[argc++] = start;
                in_arg = 0;
            }
        } else {
            if (!in_arg) {
                start = p;
                in_arg = 1;
            }
        }
        p++;
    }
    
    if (in_arg && argc < 16) {
        argv[argc++] = start;
    }
    
    if (argc == 0) return 0;
    
    for (int i = 0; i < command_count; i++) {
        if (strcmp(argv[0], command_registry[i].name) == 0) {
            return command_registry[i].func(argc, argv);
        }
    }
    
    screen_write_string("Unknown command: ");
    screen_write_string(argv[0]);
    screen_write_string("\n");
    return -1;
}

void commands_list(void) {
    for (int i = 0; i < command_count; i++) {
        screen_write_string(command_registry[i].name);
        screen_write_string(" - ");
        screen_write_string(command_registry[i].help);
        screen_write_string("\n");
    }
}
