#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include "../include/multiboot.h"

extern void kernel_main(uint32_t magic, multiboot_info_t *mboot_info);

#endif /* KERNEL_H */
