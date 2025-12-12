// kernel.c
#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "mouse.h"

void kmain(void) {
    // Inicializar IDT y controladores
    idt_init();
    mouse_init();

    // Mensaje inicial
    puts("Kernel iniciado en x86_64\n");

    // Loop principal
    while (1) {
        asm volatile("hlt"); // espera interrupciones
    }
}
