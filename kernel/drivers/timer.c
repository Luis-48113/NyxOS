#include "timer.h"

static uint32_t timer_ticks = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_init(uint32_t frequency) {
    if (frequency == 0) frequency = 100;
    
    uint32_t divisor = 1193180 / frequency;
    
    outb(TIMER_CMD_PORT, 0x36);
    outb(TIMER_CH0_PORT, divisor & 0xFF);
    outb(TIMER_CH0_PORT, (divisor >> 8) & 0xFF);
    
    timer_ticks = 0;
}

uint32_t timer_get_ticks(void) {
    return timer_ticks;
}

void timer_wait(uint32_t milliseconds) {
    uint32_t start_ticks = timer_ticks;
    uint32_t target_ticks = start_ticks + (milliseconds * 100) / 1000;  // Assuming 100Hz timer
    
    while (timer_ticks < target_ticks) {
        __asm__ volatile("hlt");  // Wait for interrupt
    }
}

void timer_handler(void) {
    timer_ticks++;
}
