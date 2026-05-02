#include "irq.h"
#include "idt.h"
#include "../../../lib/string.h"

static void (*irq_handlers[16])(void) = {NULL};
static uint8_t irq_mask = 0xFF;

static void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_CMD, 0x20);
    }
    outb(PIC1_CMD, 0x20);
}

void irq_init(void) {
    /* Remap PICs */
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
    
    /* Set IDT gates */
    idt_set_gate(32, (uint32_t)irq_handler_0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq_handler_1, 0x08, 0x8E);
}

void irq_set_handler(uint8_t irq, irq_handler_t handler) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_enable(uint8_t irq) {
    if (irq < 16) {
        irq_mask &= ~(1 << irq);
        if (irq < 8) {
            outb(PIC1_DATA, irq_mask & 0xFF);
        } else {
            outb(PIC2_DATA, (irq_mask >> 8) & 0xFF);
        }
    }
}

void irq_disable(uint8_t irq) {
    if (irq < 16) {
        irq_mask |= (1 << irq);
        if (irq < 8) {
            outb(PIC1_DATA, irq_mask & 0xFF);
        } else {
            outb(PIC2_DATA, (irq_mask >> 8) & 0xFF);
        }
    }
}

void irq_handler(uint8_t irq) {
    if (irq < 16 && irq_handlers[irq]) {
        irq_handlers[irq]();
    }
    pic_send_eoi(irq);
}
