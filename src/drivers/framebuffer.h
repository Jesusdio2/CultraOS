#pragma once
#include <stdint.h>
#include "multiboot2.h"

// Variables globales del framebuffer
extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint32_t fb_pitch;
extern void *framebuffer;

// Funciones del framebuffer
void fb_init(struct multiboot_tag_framebuffer *tag);
void fb_putpixel(int x, int y, uint32_t color);
void fb_clear(uint32_t color);
