#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    char vendor[13];
    char brand[49];
    uint32_t family;
    uint32_t model;
    uint32_t stepping;
    uint32_t base_freq;
    uint32_t max_freq;
} cpu_info_t;

void cpu_init(void);
cpu_info_t* cpu_get_info(void);
void cpu_get_vendor(char *buf, size_t len);
void cpu_get_brand(char *buf, size_t len);
uint32_t cpu_get_frequency(void);

#endif /* CPU_H */
