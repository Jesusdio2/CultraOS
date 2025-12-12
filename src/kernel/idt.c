// src/kernel/idt.c
#include "idt.h"

void idt_init(void) {
    // TODO: implementar inicialización de la IDT en 64 bits
}

void idt_set_isr(int vec, void (*handler)(void)) {
    // TODO: registrar un handler en la tabla de interrupciones
}
