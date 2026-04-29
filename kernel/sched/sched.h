#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>

typedef struct task {
    uint32_t id;
    void (*entry)(void);
    int active;
} task_t;

void sched_init(void);
void sched_add_task(uint32_t id, void (*entry)(void));
void sched_run(void);

#endif /* SCHED_H */
