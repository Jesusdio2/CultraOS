// kmain.c
#include <stdint.h>
#include "fb.h"
#include "cursor.h"
#include "mouse.h"
#include "time.h"

void kmain(uint32_t *gop_fb, int w, int h, int pitch){
  fb_init(gop_fb, w, h, pitch);

  // Prepara backbuffer: reemplaza fb_init para reservar memoria
  // Si no tienes allocador aún, reserva estático: back = static_buffer;

  cursor_init(w/2, h/2);
  mouse_init();
  pit_init(1000); // 1 kHz => sleep_ms exacto

  const int target_ms = 1000/60; // ~60 FPS

  while(1){
    fb_clear((Color){32,32,32,255}); // fondo gris
    // Dibuja UI básica si quieres
    // fb_fill_rect(20,20,200,100,(Color){50,60,80,255});

    // Dibuja cursor
    cursor_draw();

    fb_present();

    sleep_ms(target_ms);
  }
}
