#include <stdint.h>
#include <stddef.h>
#include "multiboot2.h"

uint32_t fb_width, fb_height, fb_pitch;
void *framebuffer;

void fb_init(struct multiboot_tag_framebuffer *tag) {
    fb_width  = tag->framebuffer_width;
    fb_height = tag->framebuffer_height;
    fb_pitch  = tag->framebuffer_pitch;
    framebuffer = (void*)(uintptr_t)tag->framebuffer_addr;
}

void fb_putpixel(int x, int y, uint32_t color) {
    uint8_t *row = (uint8_t*)framebuffer + (size_t)y * fb_pitch;
    uint32_t *px = (uint32_t*)(row + x * 4); // 32 bits por pixel
    *px = color;
}

void fb_clear(uint32_t color) {
    for (uint32_t y = 0; y < fb_height; y++) {
        uint32_t *row = (uint32_t*)((uint8_t*)framebuffer + (size_t)y * fb_pitch);
        for (uint32_t x = 0; x < fb_width; x++) {
            row[x] = color;
        }
    }
}
