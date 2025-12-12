// src/kernel/console.c
#include <stdint.h>

volatile char *video = (volatile char*)0xB8000;

void kputs(const char *s) {
    int i = 0;
    while (s[i]) {
        video[i*2] = s[i];
        video[i*2+1] = 0x07; // gris sobre negro
        i++;
    }
}
