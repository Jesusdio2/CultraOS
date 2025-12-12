// time.c
#include "time.h"
#include "io.h"

static volatile uint64_t g_ticks=0;

void pit_init(uint32_t hz){
  uint32_t divisor = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, (divisor>>8) & 0xFF);
  // Configura IRQ0 handler que incremente g_ticks
}

void pit_irq_handler(void){
  g_ticks++;
  // EOI al PIC
  outb(0x20, 0x20);
}

uint64_t ticks(void){ return g_ticks; }

void sleep_ms(uint32_t ms){
  uint64_t start = g_ticks;
  uint64_t target = start + ms; // si PIT en 1000 Hz, 1 tick = 1 ms
  while(g_ticks < target) { __asm__ volatile("hlt"); }
}
