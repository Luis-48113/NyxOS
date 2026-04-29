#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER_CMD_PORT  0x43
#define TIMER_CH0_PORT  0x40

void timer_init(uint32_t frequency);
uint32_t timer_get_ticks(void);
void timer_wait(uint32_t milliseconds);
void timer_handler(void);

#endif /* TIMER_H */
