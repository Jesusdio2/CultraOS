// time.h
#pragma once
#include <stdint.h>
void pit_init(uint32_t hz);
void sleep_ms(uint32_t ms);
uint64_t ticks(void);
