// fb.c
#include "fb.h"
#include <string.h>

uint32_t *fb=0, *back=0;
int fb_width=0, fb_height=0, fb_pitch=0;

void fb_init(uint32_t *framebuffer, int width, int height, int pitch){
  fb = framebuffer; fb_width=width; fb_height=height; fb_pitch=pitch;
  back = (uint32_t*) /* reserva memoria contigua del kernel */
         /* Por simplicidad, si tienes un allocador, usa malloc/pmalloc */
         /* Aquí asume una sección .bss grande o un buffer estático */
         0; // REEMPLAZA con tu allocador
}

static inline void putpx(int x,int y, Color c){
  if(x<0||y<0||x>=fb_width||y>=fb_height) return;
  uint32_t v = (c.a<<24)|(c.r<<16)|(c.g<<8)|c.b;
  uint32_t *dst = (uint32_t*)((uint8_t*)back + y*fb_pitch);
  dst[x] = v;
}

void fb_clear(Color c){
  for(int y=0;y<fb_height;y++){
    uint32_t *row = (uint32_t*)((uint8_t*)back + y*fb_pitch);
    for(int x=0;x<fb_width;x++){
      row[x] = (c.a<<24)|(c.r<<16)|(c.g<<8)|c.b;
    }
  }
}

void fb_fill_rect(int x,int y,int w,int h, Color c){
  for(int j=0;j<h;j++){
    int ty = y+j; if(ty<0||ty>=fb_height) continue;
    uint32_t *row = (uint32_t*)((uint8_t*)back + ty*fb_pitch);
    for(int i=0;i<w;i++){
      int tx = x+i; if(tx<0||tx>=fb_width) continue;
      row[tx] = (c.a<<24)|(c.r<<16)|(c.g<<8)|c.b;
    }
  }
}

void fb_present(void){
  for(int y=0;y<fb_height;y++){
    uint32_t *dst = (uint32_t*)((uint8_t*)fb + y*fb_pitch);
    uint32_t *src = (uint32_t*)((uint8_t*)back + y*fb_pitch);
    memcpy(dst, src, fb_pitch);
  }
}
