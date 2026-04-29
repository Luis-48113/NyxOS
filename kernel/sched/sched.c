#include "sched.h"
#include "../mm/memory.h"

#define MAX_TASKS 32

static task_t tasks[MAX_TASKS];
static uint32_t task_count = 0;
static int sched_initialized = 0;

void sched_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].id = 0;
        tasks[i].entry = NULL;
        tasks[i].active = 0;
    }
    task_count = 0;
    sched_initialized = 1;
}

void sched_add_task(uint32_t id, void (*entry)(void)) {
    if (!sched_initialized) sched_init();
    
    if (task_count >= MAX_TASKS) return;
    
    tasks[task_count].id = id;
    tasks[task_count].entry = entry;
    tasks[task_count].active = 1;
    task_count++;
}

void sched_run(void) {
    if (!sched_initialized) sched_init();
    
    for (uint32_t i = 0; i < task_count; i++) {
        if (tasks[i].active && tasks[i].entry) {
            tasks[i].entry();
        }
    }
}
