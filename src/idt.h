// idt.h
#pragma once
#include <stdint.h>
void idt_init(void);
void idt_set_isr(int vec, void (*handler)(void));
