#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

#define ICW1_ICW4      0x01
#define ICW1_INIT      0x10
#define ICW4_8086      0x01

typedef void (*irq_handler_t)(void);

void irq_init(void);
void irq_set_handler(uint8_t irq, irq_handler_t handler);
void irq_enable(uint8_t irq);
void irq_disable(uint8_t irq);
void irq_handler(uint8_t irq);

extern void irq_handler_0(void);
extern void irq_handler_1(void);
extern void irq_handler_2(void);
extern void irq_handler_3(void);

#endif /* IRQ_H */
