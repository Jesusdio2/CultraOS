#include "cursor.h"
#include "framebuffer.h"
#include <stdint.h>
#include <stddef.h>   // para size_t

typedef struct {
    uint8_t r, g, b, a;
} Color;

static int cx = 100, cy = 100;

#define CW 10
#define CH 16

static uint32_t icon[CH][CW]; // pequeño sprite tipo flecha

void cursor_init(int start_x, int start_y) {
    cx = start_x; cy = start_y;
    for (int y = 0; y < CH; y++) {
        for (int x = 0; x < CW; x++) {
            int is_tip = (x < 3 && y < 10 && x <= y);
            Color c = is_tip ? (Color){255,255,255,255} : (Color){0,0,0,0};
            icon[y][x] = ((uint32_t)c.a<<24) | ((uint32_t)c.r<<16) | ((uint32_t)c.g<<8) | c.b;
        }
    }
}

void cursor_set(int x, int y) {
    cx = x; cy = y;
    if (cx < 0) cx = 0;
    if (cy < 0) cy = 0;
    if (cx > (int)fb_width - 1)  cx = (int)fb_width - 1;
    if (cy > (int)fb_height - 1) cy = (int)fb_height - 1;
}

void cursor_move(int dx, int dy) { cursor_set(cx + dx, cy + dy); }

void cursor_draw(void) {
    for (int y = 0; y < CH; y++) {
        int ty = cy + y;
        if (ty < 0 || ty >= (int)fb_height) continue;
        uint32_t *row = (uint32_t*)((uint8_t*)framebuffer + (size_t)ty * fb_pitch);
        for (int x = 0; x < CW; x++) {
            int tx = cx + x;
            if (tx < 0 || tx >= (int)fb_width) continue;
            uint32_t px = icon[y][x];
            if ((px >> 24) & 0xFF) row[tx] = px;
        }
    }
}

int cursor_x(void) { return cx; }
int cursor_y(void) { return cy; }
