#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "console.h"
#include "multiboot2.h"
#include "../drivers/framebuffer.h"
#include "../drivers/mouse.h"
#include "../drivers/cursor.h"

void kmain(uint32_t magic, uint32_t addr) {
    idt_init();
    mouse_init();
    puts("Kernel iniciado en x86_64\n");

    if (magic == MULTIBOOT2_BOOTLOADER_MAGIC) {
        struct multiboot_tag *tag;
        // addr es dirección 32-bit; usa uintptr_t para aritmética de punteros segura
        for (tag = (struct multiboot_tag*)((uintptr_t)addr + 8);
             tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {

            if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
                fb_init((struct multiboot_tag_framebuffer*)tag);
                fb_clear(0x000000);
                cursor_init((int)fb_width / 2, (int)fb_height / 2);
                cursor_draw();
            }
        }
    }

    const char *msg = "Hola desde mi kernel en C!\n";
    volatile char *video = (volatile char*)0xB8000;
    for (int i = 0; msg[i] != 0; i++) {
        video[i*2] = msg[i];
        video[i*2+1] = 0x07;
    }

    while (1) { asm volatile("hlt"); }
}
