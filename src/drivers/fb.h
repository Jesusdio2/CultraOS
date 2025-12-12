// fb.h
#pragma once
#include <stdint.h>

typedef struct { uint8_t r,g,b,a; } Color;
extern uint32_t *fb;     // framebuffer de video
extern uint32_t *back;   // backbuffer
extern int fb_width, fb_height, fb_pitch; // pitch en bytes

void fb_init(uint32_t *framebuffer, int width, int height, int pitch);
void fb_clear(Color c);
void fb_fill_rect(int x,int y,int w,int h, Color c);
void fb_present(void);
