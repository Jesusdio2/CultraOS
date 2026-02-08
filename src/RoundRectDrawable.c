#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Color RGBA
typedef struct {
    uint8_t r, g, b, a;
} Color;

// Card background
typedef struct {
    float radius;
    float elevation;
    float padding;
    Color color;
} RoundRectDrawable;

// Framebuffer simulado
typedef struct {
    int width, height;
    uint32_t *pixels;
} Framebuffer;

// Función auxiliar para componer color con alfa
static inline uint32_t blend(Color c, float alpha) {
    int a = (int)(c.a * alpha);
    return (a << 24) | (c.r << 16) | (c.g << 8) | c.b;
}

// Dibuja un rectángulo redondeado con antialiasing
void draw_round_rect_antialiased(Framebuffer *fb, int x, int y, int w, int h, float radius, Color color) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            float dx = fmaxf(fmaxf(x+radius - i, 0), fmaxf(i-(x+w-radius), 0));
            float dy = fmaxf(fmaxf(y+radius - j, 0), fmaxf(j-(y+h-radius), 0));
            float dist2 = dx*dx + dy*dy;
            float alpha = 1.0f;
            if (dist2 > radius*radius) continue;
            if (dist2 > (radius-1)*(radius-1)) {
                alpha = 0.5f; // suavizado en el borde
            }
            fb->pixels[j*fb->width + i] = blend(color, alpha);
        }
    }
}

// Aplica un desenfoque gaussiano simple a la sombra
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

// Dibuja sombra estilo "elevación" con desenfoque
void draw_shadow(Framebuffer *fb, int x, int y, int w, int h, float radius, float elevation) {
    Color shadow = {0,0,0,80}; // negro translúcido
    draw_round_rect_antialiased(fb, x+elevation, y+elevation, w, h, radius, shadow);
    blur_shadow(fb, x+elevation, y+elevation, w, h, 3);
}

// Inicializa y dibuja una CardView
void draw_card(Framebuffer *fb, int x, int y, int w, int h, RoundRectDrawable *bg) {
    draw_shadow(fb, x, y, w, h, bg->radius, bg->elevation);
    draw_round_rect_antialiased(fb, x, y, w, h, bg->radius, bg->color);
}

int main() {
    Framebuffer fb;
    fb.width = 800;
    fb.height = 600;
    fb.pixels = calloc(fb.width * fb.height, sizeof(uint32_t));

    RoundRectDrawable card = {16.0f, 8.0f, 4.0f, {255,255,255,255}};
    draw_card(&fb, 100, 100, 300, 200, &card);

    // Aquí escribirías fb.pixels en /dev/fb0 o lo guardarías como PNG
    free(fb.pixels);
    return 0;
}
