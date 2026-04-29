#include "memory.h"
#include "../../lib/string.h"

static uint8_t heap[HEAP_SIZE];
static mem_block_t *heap_head = NULL;
static int mem_initialized = 0;

void mem_init(void) {
    if (mem_initialized) return;
    
    heap_head = (mem_block_t *)heap;
    heap_head->size = HEAP_SIZE - sizeof(mem_block_t);
    heap_head->free = 1;
    heap_head->next = NULL;
    heap_head->prev = NULL;
    
    mem_initialized = 1;
}

static size_t align(size_t size) {
    return (size + (HEAP_ALIGNMENT - 1)) & ~(HEAP_ALIGNMENT - 1);
}

void* malloc(size_t size) {
    if (!mem_initialized) mem_init();
    
    if (size == 0) return NULL;
    
    size = align(size);
    mem_block_t *block = heap_head;
    
    while (block != NULL) {
        if (block->free && block->size >= size) {
            if (block->size > size + sizeof(mem_block_t)) {
                mem_block_t *new_block = (mem_block_t *)((uint8_t *)block + sizeof(mem_block_t) + size);
                new_block->size = block->size - size - sizeof(mem_block_t);
                new_block->free = 1;
                new_block->next = block->next;
                new_block->prev = block;
                
                if (block->next) block->next->prev = new_block;
                
                block->size = size;
                block->next = new_block;
            }
            
            block->free = 0;
            return (void *)((uint8_t *)block + sizeof(mem_block_t));
        }
        block = block->next;
    }
    
    return NULL;
}

void free(void *ptr) {
    if (!ptr || !mem_initialized) return;
    
    mem_block_t *block = (mem_block_t *)((uint8_t *)ptr - sizeof(mem_block_t));
    block->free = 1;
    
    /* Coalesce with next free block */
    if (block->next && block->next->free) {
        block->size += sizeof(mem_block_t) + block->next->size;
        block->next = block->next->next;
        if (block->next) block->next->prev = block;
    }
    
    /* Coalesce with previous free block */
    if (block->prev && block->prev->free) {
        block->prev->size += sizeof(mem_block_t) + block->size;
        block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;
    }
}

void mem_get_stats(size_t *total, size_t *used, size_t *free_mem) {
    if (!mem_initialized) mem_init();
    
    *total = HEAP_SIZE;
    *used = 0;
    *free_mem = 0;
    
    mem_block_t *block = heap_head;
    while (block != NULL) {
        if (block->free) {
            *free_mem += block->size;
        } else {
            *used += block->size;
        }
        block = block->next;
    }
}

void mem_dump_info(void) {
    if (!mem_initialized) mem_init();
    
    size_t total, used, free_mem;
    mem_get_stats(&total, &used, &free_mem);
}
