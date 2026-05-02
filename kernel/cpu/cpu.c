#include "cpu.h"
#include "../../lib/string.h"

static cpu_info_t cpu_info;
static int cpu_initialized = 0;

static inline void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(eax) : "memory");
}

static inline void cpuid_ext(uint32_t eax, uint32_t ecx, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(eax), "c"(ecx) : "memory");
}

void cpu_init(void) {
    uint32_t eax, ebx, ecx, edx;
    
    if (cpu_initialized) return;
    
    /* Get vendor ID */
    cpuid(0, &eax, &ebx, &ecx, &edx);
    *(uint32_t *)&cpu_info.vendor[0] = ebx;
    *(uint32_t *)&cpu_info.vendor[4] = edx;
    *(uint32_t *)&cpu_info.vendor[8] = ecx;
    cpu_info.vendor[12] = '\0';
    
    /* Get brand string */
    cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
    *(uint32_t *)&cpu_info.brand[0] = eax;
    *(uint32_t *)&cpu_info.brand[4] = ebx;
    *(uint32_t *)&cpu_info.brand[8] = ecx;
    *(uint32_t *)&cpu_info.brand[12] = edx;
    
    cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
    *(uint32_t *)&cpu_info.brand[16] = eax;
    *(uint32_t *)&cpu_info.brand[20] = ebx;
    *(uint32_t *)&cpu_info.brand[24] = ecx;
    *(uint32_t *)&cpu_info.brand[28] = edx;
    
    cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
    *(uint32_t *)&cpu_info.brand[32] = eax;
    *(uint32_t *)&cpu_info.brand[36] = ebx;
    *(uint32_t *)&cpu_info.brand[40] = ecx;
    *(uint32_t *)&cpu_info.brand[44] = edx;
    cpu_info.brand[48] = '\0';
    
    /* Get family/model/stepping */
    cpuid(1, &eax, &ebx, &ecx, &edx);
    cpu_info.stepping = eax & 0xF;
    cpu_info.model = (eax >> 4) & 0xF;
    cpu_info.family = (eax >> 8) & 0xF;
    
    /* Try to get frequency information from brand string */
    cpu_info.base_freq = 0;
    cpu_info.max_freq = 0;
    
    /* Check for frequency in brand string (e.g., "@ 2.40GHz") */
    const char *brand = cpu_info.brand;
    for (int i = 0; brand[i] != '\0'; i++) {
        if (brand[i] == '@' && brand[i+1] == ' ') {
            /* Found frequency marker, extract it */
            uint32_t major = 0, minor = 0;
            int j = i + 2;
            while (brand[j] >= '0' && brand[j] <= '9') {
                major = major * 10 + (brand[j] - '0');
                j++;
            }
            if (brand[j] == '.') {
                j++;
                while (brand[j] >= '0' && brand[j] <= '9') {
                    minor = minor * 10 + (brand[j] - '0');
                    j++;
                }
            }
            cpu_info.base_freq = major * 1000 + minor * 100;
            cpu_info.max_freq = cpu_info.base_freq;
            break;
        }
    }
    
    cpu_initialized = 1;
}

cpu_info_t* cpu_get_info(void) {
    if (!cpu_initialized) cpu_init();
    return &cpu_info;
}

void cpu_get_vendor(char *buf, size_t len) {
    if (!cpu_initialized) cpu_init();
    size_t src_len = strlen(cpu_info.vendor);
    if (src_len > len - 1) src_len = len - 1;
    strncpy(buf, cpu_info.vendor, src_len);
    buf[src_len] = '\0';
}

void cpu_get_brand(char *buf, size_t len) {
    if (!cpu_initialized) cpu_init();
    size_t src_len = strlen(cpu_info.brand);
    if (src_len > len - 1) src_len = len - 1;
    strncpy(buf, cpu_info.brand, src_len);
    buf[src_len] = '\0';
}

uint32_t cpu_get_frequency(void) {
    if (!cpu_initialized) cpu_init();
    return cpu_info.base_freq;
}
