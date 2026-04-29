#include "fs.h"
#include "../../lib/string.h"
#include "../drivers/screen.h"

static const char readme_content[] = 
    "NyxOS v1-STABLE\n"
    "===============\n"
    "A hobby 32-bit OS written in x86 asm/C.\n"
    "Use 'help' for command list.\n";

static const char version_content[] = "1.0.0-STABLE";

static const char ram_content[] = "RAM Device Interface";

static fs_file_t fs_files[] = {
    {"/readme", readme_content, sizeof(readme_content) - 1},
    {"/version", version_content, sizeof(version_content) - 1},
    {"/ram", ram_content, sizeof(ram_content) - 1},
    {NULL, NULL, 0}
};

static int fs_initialized = 0;

void fs_init(void) {
    fs_initialized = 1;
}

const char* fs_read(const char *path, size_t *out_len) {
    if (!fs_initialized) fs_init();
    if (!path || !out_len) return NULL;
    
    for (int i = 0; fs_files[i].name != NULL; i++) {
        if (strcmp(path, fs_files[i].name) == 0) {
            *out_len = fs_files[i].size;
            return fs_files[i].content;
        }
    }
    
    return NULL;
}

void fs_list(void) {
    if (!fs_initialized) fs_init();
    
    for (int i = 0; fs_files[i].name != NULL; i++) {
        screen_write_string(fs_files[i].name);
        screen_write_string(" (");
        screen_write_string("?");
        screen_write_string(" bytes)\n");
    }
}

int fs_exists(const char *path) {
    if (!fs_initialized) fs_init();
    if (!path) return 0;
    
    for (int i = 0; fs_files[i].name != NULL; i++) {
        if (strcmp(path, fs_files[i].name) == 0) {
            return 1;
        }
    }
    
    return 0;
}
