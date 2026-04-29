#include "idt.h"
#include "../../../include/stddef.h"

#define IDT_ENTRIES 256

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_lo = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].type_attr = flags;
    idt[num].offset_hi = (base >> 16) & 0xFFFF;
}

void idt_init(void) {
    idt_ptr.base = (uint32_t)&idt;
    idt_ptr.size = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0x08, 0x8E);
    }
    
    load_idt(&idt_ptr);
}
