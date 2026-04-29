#ifndef FS_H
#define FS_H

#include <stdint.h>
#include "../../include/stddef.h"

typedef struct {
    const char *name;
    const char *content;
    size_t size;
} fs_file_t;

void fs_init(void);
const char* fs_read(const char *path, size_t *out_len);
void fs_list(void);
int fs_exists(const char *path);

#endif /* FS_H */
