#ifndef COMMANDS_H
#define COMMANDS_H

typedef int (*command_func_t)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    command_func_t func;
} command_t;

void commands_init(void);
void commands_register(const char *name, const char *help, command_func_t func);
int commands_execute(const char *cmdline);
void commands_list(void);

#endif /* COMMANDS_H */
