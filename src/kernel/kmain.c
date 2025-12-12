// src/kernel/kmain.c
#include <stdint.h>

void kmain(void) {
    const char *msg = "Hola desde mi kernel en C!\n";
    volatile char *video = (volatile char*)0xB8000; // memoria de texto VGA
    for (int i = 0; msg[i] != 0; i++) {
        video[i*2] = msg[i];
        video[i*2+1] = 0x07; // color gris sobre negro
    }
    while (1) { __asm__ volatile("hlt"); }
}
