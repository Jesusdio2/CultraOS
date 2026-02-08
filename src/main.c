// src/main.c
#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int fd;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    size_t screensize;
    uint8_t *fbp;
} FB;

typedef struct {
    unsigned w, h;
    uint8_t *rgba; // w*h*4 (R,G,B,A)
} Image;

// Abrir framebuffer y mapearlo
static int fb_open(FB *fb) {
    fb->fd = open("/dev/fb0", O_RDWR);
    if (fb->fd < 0) { perror("open /dev/fb0"); return -1; }
    if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->finfo) == -1) { perror("FBIOGET_FSCREENINFO"); return -1; }
    if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo) == -1) { perror("FBIOGET_VSCREENINFO"); return -1; }
    fb->screensize = fb->vinfo.yres_virtual * fb->finfo.line_length;
    fb->fbp = mmap(NULL, fb->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
    if (fb->fbp == MAP_FAILED) { perror("mmap framebuffer"); return -1; }
    return 0;
}

// Escribir un pixel con alpha-blending
static void putpixel(FB *fb, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (x < 0 || y < 0 || x >= (int)fb->vinfo.xres || y >= (int)fb->vinfo.yres) return;
    long loc = y * fb->finfo.line_length + x * (fb->vinfo.bits_per_pixel / 8);
    uint8_t *p = fb->fbp + loc;

    if (fb->vinfo.bits_per_pixel == 32) {
        uint8_t dr = p[fb->vinfo.red.offset / 8];
        uint8_t dg = p[fb->vinfo.green.offset / 8];
        uint8_t db = p[fb->vinfo.blue.offset / 8];
        float af = a / 255.0f;
        uint8_t nr = (uint8_t)(dr * (1 - af) + r * af);
        uint8_t ng = (uint8_t)(dg * (1 - af) + g * af);
        uint8_t nb = (uint8_t)(db * (1 - af) + b * af);
        p[fb->vinfo.red.offset   / 8] = nr;
        p[fb->vinfo.green.offset / 8] = ng;
        p[fb->vinfo.blue.offset  / 8] = nb;
    } else if (fb->vinfo.bits_per_pixel == 16) {
        uint16_t dst = *(uint16_t *)p;
        uint8_t dr = ((dst >> 11) & 0x1F) << 3;
        uint8_t dg = ((dst >> 5)  & 0x3F) << 2;
        uint8_t db = ( dst        & 0x1F) << 3;
        float af = a / 255.0f;
        uint8_t nr = (uint8_t)(dr * (1 - af) + r * af);
        uint8_t ng = (uint8_t)(dg * (1 - af) + g * af);
        uint8_t nb = (uint8_t)(db * (1 - af) + b * af);
        uint16_t out = ((nr >> 3) << 11) | ((ng >> 2) << 5) | ((nb >> 3));
        *(uint16_t *)p = out;
    }
}

// Limpiar pantalla
static void clear(FB *fb, uint8_t r, uint8_t g, uint8_t b) {
    for (unsigned y = 0; y < fb->vinfo.yres; ++y)
        for (unsigned x = 0; x < fb->vinfo.xres; ++x)
            putpixel(fb, x, y, r, g, b, 255);
}

// Cargar PNG en RGBA
static int load_png_rgba(const char *path, Image *img) {
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror("fopen"); return -1; }
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (!png || !info) { fclose(fp); return -1; }
    if (setjmp(png_jmpbuf(png))) { png_destroy_read_struct(&png, &info, NULL); fclose(fp); return -1; }
    png_init_io(png, fp);
    png_read_info(png, info);
    png_uint_32 w = png_get_image_width(png, info);
    png_uint_32 h = png_get_image_height(png, info);
    png_byte ct = png_get_color_type(png, info);
    png_byte bd = png_get_bit_depth(png, info);
    if (bd == 16) png_set_strip_16(png);
    if (ct == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (ct == PNG_COLOR_TYPE_GRAY && bd < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (ct == PNG_COLOR_TYPE_RGB || ct == PNG_COLOR_TYPE_GRAY || ct == PNG_COLOR_TYPE_PALETTE) png_set_add_alpha(png, 0xFF, PNG_FILLER_AFTER);
    if (ct == PNG_COLOR_TYPE_GRAY || ct == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);
    png_read_update_info(png, info);
    img->w = w; img->h = h;
    size_t stride = png_get_rowbytes(png, info);
    img->rgba = (uint8_t *)malloc(h * stride);
    png_bytep *rows = (png_bytep *)malloc(sizeof(png_bytep) * h);
    for (png_uint_32 y = 0; y < h; ++y) rows[y] = img->rgba + y * stride;
    png_read_image(png, rows);
    png_read_end(png, info);
    free(rows);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return 0;
}

// Dibujar imagen RGBA
static void draw_image(FB *fb, Image *img, int mx, int my) {
    for (unsigned py = 0; py < img->h; ++py)
        for (unsigned px = 0; px < img->w; ++px) {
            unsigned idx = (py * img->w + px) * 4;
            putpixel(fb, mx + (int)px, my + (int)py,
                     img->rgba[idx+0], img->rgba[idx+1],
                     img->rgba[idx+2], img->rgba[idx+3]);
        }
}

// Buscar dispositivo de mouse
static int find_mouse_evdev(char *out, size_t n) {
    for (int i = 0; i < 32; ++i) {
        snprintf(out, n, "/dev/input/event%d", i);
        int fd = open(out, O_RDONLY | O_NONBLOCK);
        if (fd < 0) continue;
        unsigned long evbits[(EV_MAX/8)+1];
        memset(evbits, 0, sizeof(evbits));
        if (ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits) >= 0) {
            if (evbits[EV_REL/8] & (1 << (EV_REL % 8))) return fd;
        }
        close(fd);
    }
    return -1;
}

int main() {
    FB fb;
    if (fb_open(&fb) < 0) return 1;

    Image ptr;
    if (load_png_rgba("/pointer.png", &ptr) < 0) {
        fprintf(stderr, "Failed to load pointer.png\n");
        return 1;
    }

    char devpath[64];
    int mfd = find_mouse_evdev(devpath, sizeof(devpath));
    if (mfd < 0) perror("mouse evdev not found");

    int mx = fb.vinfo.xres / 2;
    int my = fb.vinfo.yres / 2;

    clear(&fb, 0, 0, 0);

    for (;;) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Procesar eventos del mouse
        struct input_event ev;
        while (mfd >= 0 && read(mfd, &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_REL) {
                if (ev.code == REL_X) mx += ev.value;
                if (ev.code == REL_Y) my += ev.value;
            } else if (ev.type == EV_ABS) {
                if (ev.code == ABS_X) mx = ev.value;
                if (ev.code == ABS_Y) my = ev.value;
            }
        }

        // Limitar a pantalla
        if (mx < 0) mx = 0;
        if (my < 0) my = 0;
        if (mx > (int)fb.vinfo.xres - (int)ptr.w) mx = fb.vinfo.xres - ptr.w;
        if (my > (int)fb.vinfo.yres - (int)ptr.h) my = fb.vinfo.yres - ptr.h;

        // Restaurar fondo debajo del cursor (negro simple)
        for (unsigned py = 0; py < ptr.h; ++py)
            for (unsigned px = 0; px < ptr.w; ++px)
                putpixel(&fb, mx + (int)px, my + (int)py, 0, 0, 0, 255);

        // Dibujar puntero con alpha
        draw_image(&fb, &ptr, mx, my);

        // Sincronizar a ~60 FPS
        clock_gettime(CLOCK_MONOTONIC, &end);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000 +
                       (end.tv_nsec - start.tv_nsec) / 1000;
        if (elapsed < 16666) usleep(16666 - elapsed);
    }

    return 0;
}
