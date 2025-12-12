// cursor.c
#include "cursor.h"
#include "fb.h"

static int cx=100, cy=100;
static const int cw=10, ch=16;

static uint32_t icon[ch][cw]; // pequeño sprite tipo flecha

void cursor_init(int start_x,int start_y){
  cx=start_x; cy=start_y;
  // Genera un cursor simple en blanco con borde negro
  for(int y=0;y<ch;y++){
    for(int x=0;x<cw;x++){
      int is_tip = (x<3 && y<10 && x<=y);
      Color c = is_tip ? (Color){255,255,255,255} : (Color){0,0,0,0};
      icon[y][x] = (c.a<<24)|(c.r<<16)|(c.g<<8)|c.b;
    }
  }
}

void cursor_set(int x,int y){
  cx=x; cy=y;
  if(cx<0) cx=0; if(cy<0) cy=0;
  if(cx>fb_width-1) cx=fb_width-1;
  if(cy>fb_height-1) cy=fb_height-1;
}

void cursor_move(int dx,int dy){ cursor_set(cx+dx, cy+dy); }

void cursor_draw(void){
  for(int y=0;y<ch;y++){
    int ty = cy+y; if(ty<0||ty>=fb_height) continue;
    uint32_t *row = (uint32_t*)((uint8_t*)back + ty*fb_pitch);
    for(int x=0;x<cw;x++){
      int tx = cx+x; if(tx<0||tx>=fb_width) continue;
      uint32_t px = icon[y][x];
      // Composición simple: si alpha!=0, pinta
      if((px>>24)&0xFF) row[tx] = px;
    }
  }
}
