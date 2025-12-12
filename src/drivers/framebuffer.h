#pragma once
#include <stdint.h>
#include "../kernel/multiboot2.h"   // ruta correcta según tu proyecto

extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint32_t fb_pitch;
extern void *framebuffer;

void fb_init(struct multiboot_tag_framebuffer *tag);
void fb_putpixel(int x, int y, uint32_t color);
void fb_clear(uint32_t color);
