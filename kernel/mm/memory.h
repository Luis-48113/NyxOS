#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "../../include/stddef.h"

#define HEAP_SIZE (128 * 1024)
#define HEAP_ALIGNMENT 4

typedef struct memory_block {
    size_t size;
    int free;
    struct memory_block *next;
    struct memory_block *prev;
} mem_block_t;

void mem_init(void);
void* malloc(size_t size);
void free(void *ptr);
void mem_get_stats(size_t *total, size_t *used, size_t *free_mem);
void mem_dump_info(void);

#endif /* MEMORY_H */
