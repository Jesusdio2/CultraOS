#pragma once
#include <stdint.h>

typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct {
    float radius;
    float elevation;
    float padding;
    Color color;
} RoundRectDrawable;

typedef struct {
    int width, height;
    uint32_t *pixels;
} Framebuffer;

void draw_round_rect_antialiased(Framebuffer *fb, int x, int y, int w, int h, float radius, Color color);
void blur_shadow(Framebuffer *fb, int x, int y, int w, int h, int radius);
void draw_shadow(Framebuffer *fb, int x, int y, int w, int h, float radius, float elevation);
void draw_card(Framebuffer *fb, int x, int y, int w, int h, RoundRectDrawable *bg);
