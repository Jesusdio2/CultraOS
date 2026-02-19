#include "RoundRectDrawable.h"
#include <stdlib.h>
#include <math.h>

// Componer color con alfa
static inline uint32_t blend(Color c, float alpha) {
    int a = (int)(c.a * alpha);
    return (a << 24) | (c.r << 16) | (c.g << 8) | c.b;
}

void draw_round_rect_antialiased(Framebuffer *fb, int x, int y, int w, int h, float radius, Color color) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            float dx = fmaxf(fmaxf(x+radius - i, 0), fmaxf(i-(x+w-radius), 0));
            float dy = fmaxf(fmaxf(y+radius - j, 0), fmaxf(j-(y+h-radius), 0));
            float dist2 = dx*dx + dy*dy;
            float alpha = 1.0f;
            if (dist2 > radius*radius) continue;
            if (dist2 > (radius-1)*(radius-1)) alpha = 0.5f; // suavizado
            fb->pixels[j*fb->width + i] = blend(color, alpha);
        }
    }
}

void blur_shadow(Framebuffer *fb, int x, int y, int w, int h, int radius) {
    int size = fb->width * fb->height;
    uint32_t *copy = malloc(size * sizeof(uint32_t));
    for (int i = 0; i < size; i++) copy[i] = fb->pixels[i];

    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            int sum = 0, count = 0;
            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < fb->width && ny >= 0 && ny < fb->height) {
                        uint32_t p = copy[ny*fb->width + nx];
                        sum += (p >> 24) & 0xFF; // alfa
                        count++;
                    }
                }
            }
            int avg = sum / count;
            uint32_t base = fb->pixels[j*fb->width + i] & 0x00FFFFFF;
            fb->pixels[j*fb->width + i] = (avg << 24) | base;
        }
    }
    free(copy);
}

void draw_shadow(Framebuffer *fb, int x, int y, int w, int h, float radius, float elevation) {
    Color shadow = {0,0,0,80}; // negro translúcido
    draw_round_rect_antialiased(fb, x+elevation, y+elevation, w, h, radius, shadow);
    blur_shadow(fb, x+elevation, y+elevation, w, h, 3);
}

void draw_card(Framebuffer *fb, int x, int y, int w, int h, RoundRectDrawable *bg) {
    draw_shadow(fb, x, y, w, h, bg->radius, bg->elevation);
    draw_round_rect_antialiased(fb, x, y, w, h, bg->radius, bg->color);
}
