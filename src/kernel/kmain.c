#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "../drivers/mouse.h"
#include "console.h"

void kmain(void) {
    // Inicializar IDT y controladores
    idt_init();
    mouse_init();

    // Mensaje inicial en VGA usando tu propia puts
    puts("Kernel iniciado en x86_64\n");

    // También puedes escribir directamente en memoria VGA si quieres
    const char *msg = "Hola desde mi kernel en C!\n";
    volatile char *video = (volatile char*)0xB8000;
    for (int i = 0; msg[i] != 0; i++) {
        video[i*2] = msg[i];
        video[i*2+1] = 0x07; // gris sobre negro
    }

    // Loop principal
    while (1) {
        asm volatile("hlt"); // espera interrupciones
    }
}
